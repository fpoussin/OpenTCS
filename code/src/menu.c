struct menuStruct;
void mainScreen();
void item2Handle();

//Font for the arrows
const unsigned short Arrows5x6[] = {
        0x30, 0x3C, 0x3F, 0x3C, 0x30,
        0x03, 0x0F, 0x3F, 0x0F, 0x03
        };

//Menu item structure
typedef struct menuItem
               {
                char *itemName;                 //Name of this item
                void (*handler)(void);          //Pointer to function, null if not used
                struct menuStruct *subMenu;     //Pointer to sub menu, null if not used
               } menuItem;

//Menu structure
typedef struct menuStruct
               {
                char *menuName;                 //Name of menu
                unsigned short numberItems;     //Number of menu items
                menuItem *items;                //Pointer to array of items
               } menuStruct;

//Demo menu. Menus are declared in reverse order,
menuItem subItem1ItemList[] = {{"Sub Sub Item 1", 0, 0}, {"Sub Sub Item 2", 0, 0}};

menuStruct subItem1Menu = {"Sub Item 1 Sub Menu", 2, &subItem1ItemList};

menuItem item1ItemList[] = {{"Sub sub menu 1", 0, &subItem1Menu}, {"Sub Item 2", 0, 0}};

menuStruct item1Menu = {"Item 1 Sub Menu", 2, &item1ItemList};

menuItem mainMenuItemList[] = {{"Sub Menu 1", 0, &item1Menu},{"Demo function", &item2Handle, 0},{"Sub sub menu 1", 0, &subItem1Menu},{"Item 4", 0, 0},{"Item 5", 0, 0},
                               {"Item 6", 0, 0},{"Item 7", 0, 0},{"Item 8", 0, 0},{"Item 9", 0, 0},{"Item 10", 0, 0}};

menuStruct mainMenu = {"Main Menu", 10, &mainMenuItemList};

//Demo item handle function
void item2Handle()
{
 GLCD_Fill(0x00);
 GLCD_Write_Text("Press RA4", 0, 0, 1);

 while(!Button(&PORTA, 4, 5, 1))
 {
 }
 return;
}

//Draws the actual menu
void drawMenu(menuStruct *menuToShow, short selectedIndex)
{
 unsigned short i;

 //~ GLCD_Fill(0x00);
 //~ GLCD_Set_Font(FontSystem5x8,5,8,32);
 //~ GLCD_Write_Text(menuToShow->menuName, 0, 0, 1);

 for(i=0;i < (menuToShow->numberItems > 6 ? 6 : menuToShow->numberItems) ;i++)
 {
  if (i > 5 + (selectedIndex > 5 ? selectedIndex - 5 : 0))
  {
   break;
  }
  else if (selectedIndex < 6)
  {
   //~ GLCD_Write_Text(menuToShow->items[i]->itemName, 5, i+1, (selectedIndex == i) ? 0 : 1);
  }
  else
  {
   //~ GLCD_Write_Text(menuToShow->items[i+(selectedIndex-5)]->itemName, 5, i+1, (selectedIndex == i+(selectedIndex-5)) ? 0 : 1);
  }
 }

 //~ Glcd_H_Line(0,127,7,1);
 //~ Glcd_H_Line(0,127,55,1);
 //~ GLCD_Set_Font(Arrows5x6,5,6,30);
 //~ GLCD_Write_Char(30, 5, 7, 1);
 //~ GLCD_Write_Char(31, 25, 7, 1);
 //~ GLCD_Set_Font(FontSystem5x8,5,8,32);
 //~ GLCD_Write_Text("Select", 45, 7, 1);
 //~ GLCD_Write_Text("Back", 101, 7, 1);
 return;
}

//Main menu function
void openMenu(menuStruct *menuToShow)
{
 short selectedIndex = 0;               //Current selected item

 delay_ms(50);
 drawMenu(menuToShow, selectedIndex);

   do {
     if (Button(&PORTA, 0, 5, 1))
     {
        selectedIndex--;
        if (selectedIndex < 0)
        {
           selectedIndex = menuToShow->numberItems - 1;
        }
        drawMenu(menuToShow, selectedIndex);
     }
     else if (Button(&PORTA, 1, 5, 1))
     {
        selectedIndex++;
        if (selectedIndex > (menuToShow->numberItems) - 1)
        {
           selectedIndex = 0;
        }
        drawMenu(menuToShow, selectedIndex);
     }
     else if (Button(&PORTA, 2, 5, 1))
     {
         if (menuToShow->items[selectedIndex]->handler != 0)
         {
          menuToShow->items[selectedIndex]->handler();
         }
         else if (menuToShow->items[selectedIndex]->subMenu != 0)
         {
          openMenu(menuToShow->items[selectedIndex]->subMenu);
         }
         drawMenu(menuToShow, selectedIndex);
     }
     else if (Button(&PORTA, 3, 5, 1))
     {
          return;
     }

     delay_ms(30);
  } while (1);

 return;
}

void mainScreen()
{
 //~ GLCD_Fill(0x00);
 //~ GLCD_Set_Font(FontSystem5x8,5,8,32);
 //~ GLCD_Write_Text("Menu", 0, 7, 1);
 return;
}

void main()
{
  ADPCFG = 0xFFFF;

  Glcd_Init_LV_24_33();
  mainScreen();

  //--- main loop
  do
  {
    if (Button(&PORTA, 0, 1, 1))
    {
     openMenu(&mainMenu);
     mainScreen();
    }
    Delay_ms(50);
  } while (1);
}
