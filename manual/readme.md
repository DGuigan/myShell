Name
    myshell

Usage
    myshell [batchfile]

Description
    myshell is a simple command line interpreter that performs the limited number of commands listed below.

Internal Commands
    cd [path]
        If path is supplied and exists, change the current directory to path.
        If path is not supplied print the current directory.

    clr
        Clear the screen.

    dir [path]
        If path is supplied print all contents of path in long form.
        If path is not supplied print all contents of current directory in long form.

    environ
        Print all environment variables, one per line.

    echo [args...]
        If args supplied print them to a new line, multiple whitespaces and tabs are truncated to one space.
        If args not supplied print a black line.

    help
        Print this user manual using the more command.

    pause
        Suspend operation of shell until enter is pressed.

    rename <source file> <destination file>
        Rename source file to destination file. If destination file exists prompt for overwrite

    quit
        Exits the shell.
    
External Commands
    Any command not recognised as an internal command is assumed to be an external, executable.
    After its existance is verified it and its arguments are executed using the exec function.

Arguments
    Arguements are deliminated by spaces, tabs and newlines.

Environment Variables
    myshell inherits all environment variables from its parent process and sets the following custon variables:

    SHELL
        The full path to the myshell exectutable file

    MAN_PATH
        The full path to this readme file

    PARENT
        Each child process created by myshell contains a PARENT variable which corresponds to the SHELL variable above

I/O Redirection
    After the arguments for a command, a user can redirect input and output in the following ways:

    cmd [ARGS...] < inputfile
        myshell will set the input stream for cmd to inputfile

    cmd [ARGS...] > outputfile
        myshell will set the output stream for cmd to outputfile, output file will be truncated.
    
    cmd [ARGS...] >> outputfile
        myshell will set the output stream for cmd to outputfile, output file will be appended to.
    
    If multiple attempts to redirect input or output are made on a single line then the rightmost of these will be accepted and others treated as arguments to cmd.
    The redirection symbols "<", ">" and ">>" should never be given as arguments unless immediately followed by the relevent filenames.

Batchfile
    If a file is passed as an argument to myshell then the input stream will be set to that file and each line treated as input.

Background Execution
    If a user wishes myshell to perform a task in the background and immediately prompt for further input then they can include an "&" as the final argument of their input.
    An "&" given as an arguement in any position other than the last will be treated as a regular argument.



Name: David McGuigan
Student ID: 19425942

I hereby acknowledge the DCU Academic Integrity Policy
