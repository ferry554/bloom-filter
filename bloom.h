
#include <stdlib.h>
#include <stdint.h>

// m为位数组的长度，以bit记
// k为使用的哈希函数个数
struct bloom_filter
{
	uint32_t m;
	uint32_t k;
	uint8_t *bit_vector;
};

void bf_init(struct bloom_filter *bf, uint32_t m, uint32_t k);

void bf_destroy(struct bloom_filter *bf);

// 插入和查询元素key时，要传入key的地址和长度，从而适用于多种数据类型
void bf_insert(struct bloom_filter *bf, const void *key, size_t len);

int bf_may_contain(struct bloom_filter *bf, const void *key, size_t len);