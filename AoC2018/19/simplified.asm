Only if REG1 == 0

START:
 REG5 = 10551367
 REG0 = 0
 REG1 = 1

IN1:
 REG4 = 1

LOOP:
 REG2 = REG1 * REG4
 if (REG2 == REG5) REG0 += REG1
 REG4++
 if (REG4 > REG5) GOTO INC
GOTO LOOP

INC:
 REG1++
 if (REG1 > REG5) GOTO END
GOTO IN1

END: print REG0
