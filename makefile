  MagicSquare *genderateMagicSquare(int n)
- make a sqaure from input size 
- make a siamese magic sware 
-reutrn a pointer to the struct 



void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) 


free memory 
close files 









v




so i have initialized a struct 2D array for p2b in the generateMagicSquare method but when I want to set a value in the array to something, ex: 




*(*(board+row)+col) = 0;




it says this is wrong. in the notes we set values by doing pmptr -> weight = 20. but with this project we don't have weight variables or anything similar. how do we set individual values?





 MagicSquare **board;

   //int **board;
   board = (MagicSquare **)malloc(sizeof(MagicSquare *) * size);

   for(int i = 0; i < size; i++)
   {
        *(board + i) =(MagicSquare *) malloc(sizeof(MagicSquare) * size);
   }

   //checks return value of malloc
   if(board == NULL)
        {
        printf("Nothing inside board");
        exit(1);
        }
\

























