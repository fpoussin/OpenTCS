#include "nil.h"
#include "ssd1306.h"

struct __menuStruct;
typedef const struct __menuStruct menuStruct_t;

//Menu item structure
struct __menuItem
{
    char *itemName;                 //Name of this item
    void (*handler)(void);          //Pointer to function, null if not used
    menuStruct_t *subMenu;     //Pointer to sub menu, null if not used
};
typedef const struct __menuItem menuItem_t;

//Menu structure
struct __menuStruct
{
    char *menuName;                 //Name of menu
    unsigned short numberItems;     //Number of menu items
    menuItem_t *items;                //Pointer to array of items
};

void openMenu(menuStruct_t *menuToShow);
