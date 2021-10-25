# CPsudoku

[click here to download](https://www.github.com/SnailMath/CPsudoku/releases)

This is the first version of CPsudoku, a simple sudoku game and solver for the fx-cp400 aka Classpad II. You need to install the launcher [hollyhock-2](https://www.github.com/SnailMath/hollyhock-2/releases) first, to use this program. When this is installed, just put the file CPsudoku.bin onto the calc and start it using the hollyhock menu.

## Sudoku game
If you want to solve sudokus by yourself, you can use this program to generate sudokus. Use the key `Keyboard` to go into the menu (a red arrow should appear), select 'random' and click `EXE`, this will generate the already solved sudoku, select '75 percent' and click `EXE` one to three times to make the sudoku harder, and select 'set' and click `EXE` to lock these given numbers (they'll appear blue and you can't edit them anymore). Then you can switch to edit mode using the key 'Keyboard'. 
Now have fun solving your sudoku.

## Sudoku solver
If you want to 'cheat' in sudokus, you can use this program to solve them for you. First, go to the menu (using the key `Keyboard`), select 'Clear' from the menu if the field is not empty. Then go to edit mode (using the key `Keyboard` again) and type in all the numbers that are given. Then go back to the menu (`Keyboard`) and select 'Set' to lock the numbers you entered. Now select 'solve' multiple times and the sudoku should get solved. 
For solving sudokus, I use three methods, the what-can't-be-there-method, the what-should-be-there-method and the trial-and-error-method. I've only implemented the first one, but it should be able to solve most sudokus. But it might not work for every possibility, in that case just try to input some random numbers in some unsolved fields and keep pressing solve. If a field should go completely blank, you've typed in an incorrect number and now the sudoku is unsolvable, just remove a few numbers and it should be solvable again... I don't know if I'll add the other methods to solve sudokus easier...

## Check sudokus
When you've solved the sudoku or the calc has, you can select 'check' to test if it is solved correctly. 

## Have fun.
