
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// هيكل يمثل المخزن الدائري
typedef struct {
    char *buf;
    int size;
    int head;
    int tail;
} CB;

// تهيئة المخزن بحجم معين
int cb_init(CB *c, int cap) {
    if (cap <= 0) return 0;
    c->buf = malloc(cap + 1);
    if (!c->buf) return 0;
    c->size = cap;
    c->head = 0;
    c->tail = 0;
    return 1;
}

// تحرير الذاكرة
void cb_free(CB *c) {
    if (c->buf) free(c->buf);
    c->buf = NULL;
}

// التحقق إذا المخزن فارغ
int cb_empty(CB *c) {
    return c->head == c->tail;
}

// التحقق إذا المخزن ممتلئ
int cb_full(CB *c) {
    return ((c->head + 1) % (c->size + 1)) == c->tail;
}

// كتابة عنصر داخل المخزن
int cb_write(CB *c, char v) {
    if (cb_full(c)) return 0;
    c->buf[c->head] = v;
    c->head = (c->head + 1) % (c->size + 1);
    return 1;
}

// قراءة عنصر من المخزن
int cb_read(CB *c, char *out) {
    if (cb_empty(c)) return 0;
    *out = c->buf[c->tail];
    c->tail = (c->tail + 1) % (c->size + 1);
    return 1;
}

int main() {
    // قراءة الاسم من المستخدم
    char name[100];
    printf("الاسم: ");
    fgets(name, sizeof(name), stdin);

    // حذف سطر جديد من الإدخال
    if (strlen(name) > 0 && name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';

    // تكوين النص النهائي
    char full[150];
    sprintf(full, "%s CE-ESY", name);
    int len = strlen(full);

    printf("\nالناتج: %s (len=%d)\n\n", full, len);

    // تجربة أولى: مخزن صغير
    int cap1 = len - 2;
    if (cap1 <= 0) cap1 = 1;

    CB c1;
    cb_init(&c1, cap1);

    printf("عندما يكون حجم المخزن: %d:\n", cap1);

    // محاولة كتابة النص داخل مخزن صغير
    for (int i = 0; i < len; i++) {
        if (!cb_write(&c1, full[i])) {
            printf("لم يتم كتابة '%c'\n", full[i]);
        }
    }

    // قراءة ما تم تخزينه
    printf("القراءة: ");
    char ch;
    while (cb_read(&c1, &ch)) {
        printf("%c", ch);
    }

    printf("\n\n");
    cb_free(&c1);

    // تجربة ثانية: مخزن أكبر
    CB c2;
    cb_init(&c2, len + 5);

    int ok = 1;
    printf("عندما يكون حجم المخزن %d:\n", len + 5);

    // كتابة النص في مخزن أكبر
    for (int i = 0; i < len; i++) {
        if (!cb_write(&c2, full[i])) {
            ok = 0;
        }
    }

    printf("تمت الكتابة الناتج كله؟ %s\n", ok ? "اي" : "لا");

    // قراءة كامل المحتوى
    printf("القراءة: ");
    while (cb_read(&c2, &ch)) {
        putchar(ch);
    }

    printf("\n");

    cb_free(&c2);
    return 0;
}
