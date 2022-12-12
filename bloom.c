#include "bloom.h"

#include <string.h>
#include <unistd.h>

#include "murmur3.c"

#define bit_set(v, n) ((v)[(n) >> 3] |= (0x1 << (0x7 - ((n)&0x7))))
#define bit_get(v, n) ((v)[(n) >> 3] & (0x1 << (0x7 - ((n)&0x7))))
#define bit_clr(v, n) ((v)[(n) >> 3] &= ~(0x1 << (0x7 - ((n)&0x7))))

void bf_init(struct bloom_filter *bf, uint32_t m, uint32_t k)
{
	memset(bf, 0, sizeof(*bf));
	bf->m = m;
	bf->k = k;
	bf->bit_vector = calloc(m >> 3, sizeof(char));
}

void bf_destroy(struct bloom_filter *bf)
{
	free(bf->bit_vector);
}

// 插入和查询元素key时，要传入key的地址和长度，从而适用于多种数据类型
void bf_insert(struct bloom_filter *bf, const void *key, size_t len)
{
	uint32_t i;
	uint32_t hash;

	for (i = 0; i < bf->k; i++)
	{
		// 对murmurhash传入k个种子，相当于k个不同哈希函数，得到k个哈希值
		murmur3_hash32(key, len, i, &hash);

		// 哈希值限制在[0,m-1]
		hash %= bf->m;

		// 把位数组相应位置置为1
		bit_set(bf->bit_vector, hash);
	}
}

int bf_may_contain(struct bloom_filter *bf, const void *key, size_t len)
{
	uint32_t i;
	uint32_t hash;

	for (i = 0; i < bf->k; i++)
	{
		murmur3_hash32(key, len, i, &hash);
		hash %= bf->m;
		// 全1为存在，有一个为0为不存在
		if (!bit_get(bf->bit_vector, hash))
			return 0;
	}
	return 1;
}