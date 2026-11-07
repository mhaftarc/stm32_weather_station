#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RB_SIZE 8

typedef struct
{
    uint8_t tab[RB_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

void rb_init(RingBuffer *p)
{
    p->head = 0;
    p->tail = 0;
    p->count = 0;
}

bool rb_put(RingBuffer *rb, uint8_t data)
{
    if(rb->count == RB_SIZE){
        return false;
    }
    rb->tab[rb->head] = data;
    rb->head++;
    if(rb->head >= RB_SIZE){
        rb->head = 0;
    }
    rb->count++;
    return true;
}

bool rb_get(RingBuffer *rb, uint8_t *data){
    if(rb->count == 0){
        return false;
    }
    *data = rb->tab[rb->tail];
    rb->tail++;
    rb->count--;
    if(rb->tail >= RB_SIZE){
        rb->tail = 0;
    }
    return true;

}

int main(void)
{
    RingBuffer x;
    uint8_t data;

rb_init(&x);


for(int i = 0; i < 10; i++){
    rb_put(&x,data);
    printf("%d,%d,%d\n",x.head, x.tail, x.count);
        printf("data = : %d\n",data);


};
for(int i = 0; i < 10; i++){
    rb_get(&x,&data);
    printf("%d,%d,%d\n",x.head, x.tail, x.count);
    printf("data = : %d\n",data);

};

    return 0;
}