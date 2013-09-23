#include "menu.h"

//Demo menu. Menus are declared in reverse order,
/*
menuItem subItem1ItemList[] = {{"Sub Sub Item 1", 0, 0}, {"Sub Sub Item 2", 0, 0}};

menuStruct subItem1Menu = {"Sub Item 1 Sub Menu", 2, subItem1ItemList};

menuItem item1ItemList[] = {{"Sub sub menu 1", 0, &subItem1Menu}, {"Sub Item 2", 0, 0}};

menuStruct item1Menu = {"Item 1 Sub Menu", 2, item1ItemList};

menuItem mainMenuItemList[] = {{"Sub Menu 1", 0, &item1Menu},{"Demo function", &item2Handle, 0},{"Sub sub menu 1", 0, &subItem1Menu},{"Item 4", 0, 0},{"Item 5", 0, 0},
                               {"Item 6", 0, 0},{"Item 7", 0, 0},{"Item 8", 0, 0},{"Item 9", 0, 0},{"Item 10", 0, 0}};

menuStruct mainMenu = {"Main Menu", 10, mainMenuItemList};

//Demo item handle function
void item2Handle(void)
{
  ssd1306ClearScreen();
  ssd1306DrawString(0, 0, "item2Handle", Font_System5x8);

  while(BUTTON_SEL) {};

  return;
}
*/
//Draws the actual menu
void drawMenu(menuStruct_t *menuToShow, short selectedIndex)
{
 uint8_t i, y;

 ssd1306ClearScreen();
 ssd1306DrawString(0, 0, menuToShow->menuName, Font_System5x8);

 for(i=0;i < ((menuToShow->numberItems+1) > 6 ? 6 : (menuToShow->numberItems+1)) ;i++)
 {
  y = (i*7)+1;
  if (i > 5 + (selectedIndex > 5 ? selectedIndex - 5 : 0))
  {
   break;
  }
  else if (selectedIndex < 6)
  {
      if (selectedIndex == i) ssd1306DrawChar(0, y, '>', Font_System5x8);
      if (selectedIndex > menuToShow->numberItems)
      {
          ssd1306DrawString(7, y, "Go Back", Font_System5x8);
      }
      else
      {
          ssd1306DrawString(7, y, menuToShow->items[i].itemName, Font_System5x8);
      }
  }
  else
  {
      if (selectedIndex == i+(selectedIndex-5)) ssd1306DrawChar(0, y, '>', Font_System5x8);
      if (selectedIndex > menuToShow->numberItems)
      {
          ssd1306DrawString(7, y, "Go Back", Font_System5x8);
      }
      else
      {
          ssd1306DrawString(7, y, menuToShow->items[i+(selectedIndex-5)].itemName, Font_System5x8);
      }
  }
 }

 return;
}

//Main menu function
void openMenu(menuStruct_t *menuToShow)
{
 int8_t selectedIndex = 0;               //Current selected item

 chThdSleepMilliseconds(50);
 drawMenu(menuToShow, selectedIndex);

   do {
     if (BUTTON_DOWN)
     {
        selectedIndex--;
        if (selectedIndex < 0)
        {
           selectedIndex = menuToShow->numberItems + 1;
        }
        drawMenu(menuToShow, selectedIndex);
     }
     else if (BUTTON_UP)
     {
        selectedIndex++;
        if (selectedIndex > (menuToShow->numberItems + 1))
        {
           selectedIndex = 0;
        }
        drawMenu(menuToShow, selectedIndex);
     }
     else if (BUTTON_SEL)
     {
        if (selectedIndex > menuToShow->numberItems) /* Last item is "exit" */
        {
            return;
        }
        else if (menuToShow->items[selectedIndex].handler != NULL)
        {
            menuToShow->items[selectedIndex].handler();
        }
        else if (menuToShow->items[selectedIndex].subMenu != NULL)
        {
            openMenu(menuToShow->items[selectedIndex].subMenu);
        }
        drawMenu(menuToShow, selectedIndex);
     }
     chThdSleepMilliseconds(250);
  } while (true);

  return;
}
