#include <stdio.h>

int main()
{
  int s = 3;

  switch(s)
  {
    case 5,3:
      printf("reached 3\n");
    break;
    
    case 4: 
      printf("reached 4\n");
    break;
    
    default:
      printf("reached nothing\n");
    
  }

}