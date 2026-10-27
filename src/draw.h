#ifndef _DRAW_H_
#define _DRAW_H_


#define GENERAL_PAD     2

#define ELEMS           5
#define ELEM_WIDTH      100
#define CONTENT_X       (GENERAL_PAD)
#define CONTENT_Y       (BAR_WIDTH + GENERAL_PAD)
#define CONTENT_WIDTH   (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD)
#define CONTENT_HEIGHT  (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD)

#define WINDOW_WIDTH    (CONTENT_WIDTH + 2*GENERAL_PAD)
#define WINDOW_HEIGHT   (CONTENT_HEIGHT + 2*GENERAL_PAD + BAR_WIDTH)
#define BAR_WIDTH       24

#define TEXT_NUM        128
#define TEXT_MAX_LEN    64

struct Stack {
        Rectangle boundaries;
        float x, y, w, h, gap;
};

Rectangle NextRight(struct Stack *s);

struct States {
        char pool[TEXT_NUM][TEXT_MAX_LEN];
        char *slot_texts[TEXT_NUM];
        int slots[25];
};

void drawContent(struct States *states);

#endif  // _DRAW_H_