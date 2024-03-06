QBasic，SJTU SEP课程期末大作业，使用QT Creator开发的Basic解释器

简单易懂，注释齐全，可供借鉴，请勿抄袭~

走过路过点个STAR再走呗！

Table 1. Statements implemented in the minimal version of BASIC：

REM	This statement is used for comments. Any text on the line after the keyword REM is ignored. 

LET	This statement is BASIC’s assignment statement. The LET keyword is followed by a variable name, an equal sign, and an expression. As in C++, the effect of this statement is to assign the value of the expression to the variable, replacing any previous value. In BASIC, assignment is not an operator and may not be nested inside other expressions. 

PRINT	In minimal BASIC, the PRINT statement has the form: 
PRINT exp
where exp is an expression. The effect of this statement is to print the value of the expression on the console and then print a newline character so that the output from the next PRINT statement begins on a new line.

INPUT	In the minimal version of the BASIC interpreter, the INPUT statement has the form: 
INPUT var
where var is a variable read in from the user. The effect of this statement is to print a prompt consisting of the string " ? " and then to read in a value to be stored in the variable. (The string " ? " should display in the command input edit box in GUI.)

GOTO	This statement has the syntax
GOTO n
and forces an unconditional change in the control flow of the program. When the program hits this statement, the program continues from line n instead of continuing with the next statement. Your program should report an error if line n does not exist.

IF	This statement provides conditional control. The syntax for this statement is:
IF exp1 op exp2 THEN n 
where exp1 and exp2 are expressions and op is one of the conditional operators
=, <, or >. If the condition holds, the program should continue from line n just as in the GOTO statement. If not, the program continues on to the next line.
Note that the conditional operators (=, <, >) are not parts of expressions.

END	Marks the end of the program. Execution halts when this line is reached. This statement is usually optional in BASIC programs because execution also stops if the program continues past the last numbered line. 

 
 Table 2. Commands to control the BASIC interpreter
 
RUN	This command starts program execution beginning at the lowest-numbered line. Unless the flow is changed by GOTO and IF commands, statements are executed in line-number order. Execution ends when the program hits the END statement or continues past the last statement in the program. 

LOAD	This command loads a file containing statements and commands. Statements and commands should be stored (also displayed in GUI) and executed respectively, as if they were entered into input box in order. A prompt window should be displayed when this command is entered. The window asks users to choose the file to load.

LIST	This command lists the steps in the program in numerical sequence. It has been required to be implemented in the previous version of this project. In the new version, your interpreter should be able to display all the codes that have been entered in real time, so there is no need to implement this command.

CLEAR	This command deletes the program so the user can start entering a new one.

HELP	This command provides a simple help message describing your interpreter.

QUIT	Typing QUIT exits from the BASIC interpreter.

以及语法树、异常处理等更多功能！
