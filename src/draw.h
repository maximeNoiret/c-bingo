#ifndef _DRAW_H_
#define _DRAW_H_


#define GENERAL_PAD     2
#define BAR_WIDTH       24

constexpr unsigned ELEMS                = 5;
constexpr unsigned TOTAL_ELEMS          = ELEMS*ELEMS;
constexpr unsigned ELEM_WIDTH           = 100;
constexpr unsigned CONTENT_X            = GENERAL_PAD;
constexpr unsigned CONTENT_Y            = (BAR_WIDTH + GENERAL_PAD);
constexpr unsigned CONTENT_WIDTH        = (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD);
constexpr unsigned CONTENT_HEIGHT       = (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD);

constexpr unsigned WINDOW_WIDTH         = (CONTENT_WIDTH + 2*GENERAL_PAD);
constexpr unsigned WINDOW_HEIGHT        = (CONTENT_HEIGHT + 2*GENERAL_PAD + BAR_WIDTH);

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
        size_t text_num;
        int slots[TOTAL_ELEMS];
};

void drawContent(struct States *states);

#endif  // _DRAW_H_