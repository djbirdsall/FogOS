typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[sizeof(Point)*20];
    int length;
    Point direction;
} Snake;
