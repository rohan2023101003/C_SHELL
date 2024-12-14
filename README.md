
# Implementation of Basic Shell Structure In C

`my_shell` is a custom shell implementation built using C. This project demonstrates basic shell functionalities such as command execution, directory navigation, and background process handling.

## Project Structure


  - `foreground.c`: Core shell implementation for all foreground processes.
  - `main.c`: Entry point for the shell.
  - `proclore.c`: Additional functionalities related to process handling, gives information of the process.
  - `reveal.c`: Functionality for revealing or managing shell states.
  - `seek.c`: Implementation for searching or navigating directories.
  - `background.c`: Functions related to processes which have to be run in the background.
  - `display.c` : Prompts the user command containing details.
  - `log.c` : Handles storing all the commands execpt log commnds
  - `hop.c` : It hops(changes) to directories as specified
  - `activities.c` : Displays a list of processes spawned by your shell with its details
  - `bg.c` : Changes a stopped process to running in the background.
  - `fg.c` : Brings a process to the foreground.
  - `iMan.c` :Fetches man pages from the internet using sockets and prints them to the terminal.
  - `IO.c` : Supports redirection using: > , <  and >>
  - `myshrc.c` : Supports  custom shell configuration file, similar to .bashrc. 
  - `.myshrc` : You can define aliases and functions in this file to simplify commands.
  - `neonate.c` : For printing the Process ID (PID) of the most recently created process every specified interval until the key x is pressed.
  - `pipe.c` : Pipes allow the output of one command to be passed as input to another. Supports any number of pipes
  - `process.c` : It has utility fucntions and structs to store details of all processes.
  - `custom.c` : File containg utilities for custom commands
  - `sig.c` : Send signals to processes using the ping command.
- `Makefile`: Build script to compile the source files into an executable.
- `README.md`: This file.
- Each `.c` has its own `.h` header file


## Building the Project

To compile the project, you need to have `gcc` installed on your system. The project uses a Makefile to manage the build process.

1. **Clone the repository** (if applicable):
   ```sh
   git clone https://github.com/OSN-Monsoon-2024/mini-project-1-rohan2023101003.git
   ```
   ```
   cd <repository-directory>
   ```

2. **Compile the project**:
   ```sh
   make
   ```

   This command will:
   - Compile the source files into object files and place them in the `obj` directory.
   - Link the object files to create the `my_shell` executable in the current directory.



## Running the Shell

Once compiled, you can run the shell executable directly from the command line:

```sh
./my_shell
```

## Details

1. **Display Prompt:** Display a dynamic shell prompt in the format `<Username@SystemName:~>`, showing the relative path inside the home directory or absolute paths outside it, with updates upon directory change.

2. **Input Requirements:** Support multiple commands separated by `;` or `&`, handle random spaces/tabs, and ensure background commands print their process IDs.

3. **Hop Command:** Implement `hop` to change directories with support for `~`, `..`, `-`, and absolute/relative paths, displaying the new path after each change.

4. **Reveal Command:** List files/directories with `reveal`, supporting `-a` (show hidden files) and `-l` (detailed info) flags, using color coding for files (white), directories (blue), and executables (green).

5. **Log Command:** Track and display the last 15 commands entered, skipping duplicates and `log` itself, with commands executed in the correct order across sessions.

6. **System Commands:** Execute system commands in the foreground or background, print process time for commands taking longer than 2 seconds, and notify users when background processes complete.

7. **Proclore Command:** Display process information (PID, status, group, memory usage, executable path) for either the current shell or a specified process.

8. **Seek Command:** Search for files/directories with `seek`, supporting `-d` (directories), `-f` (files), and `-e` (auto-execution) flags, showing relative paths and handling permission errors.

9. **.myshrc File:** Create a custom shell config file with aliases and functions, such as `mk_hop` (create and hop into a directory) and `hop_seek` (hop and search).

10. **I/O Redirection:** Support redirection (`>`, `<`, `>>`) for input/output to/from files, handling command execution with or without redirection.

11. **Piping:** Implement piping to allow data flow between commands, chaining multiple commands in sequence using `|`.

12. **Aliases/Functions:** Handle user-defined aliases and simple functions from the `.myshrc` file, allowing custom command mappings.

13. **Signal Command:** Implement ping to send signals to processes, with signal numbers taken modulo 32. Handle signals like SIGKILL, SIGTERM, SIGINT, and transitions between process states (Running, Stopped), logging status updates when signals are sent.
- The shell will handle the following signals:
  - `Ctrl+C`: For this the current foreground process will be terminated
  - `Ctrl+D` : For this the shell will exit
   - `Ctrl+Z` : For this the current foreground process will be stopped and the shell will print the process id of the stopped process 

14. **Foreground (fg) Command:** Brings a background process with the given PID to the foreground, giving it control of the terminal. If no process with the given PID exists, prints “No such process found.”

15. **Background (bg) Command:** Resumes a stopped background process with the given PID, changing its state to running. If no process with the given PID exists, prints “No such process found.”

16. **Neonate (neonate) Command:** Prints the Process-ID of the most recently created process every `[time_arg]` seconds until the user presses 'x'. The output includes the PID printed repeatedly until termination.

17. **iMan Command:** Fetches and displays man pages from http://man.he.net/ using sockets. Outputs the raw HTML content (including tags) for the given command name, excluding the request header.
---

# Assumptions



1. `General`
- Relative paths must begin with ``` ~/```
- Maximum file size(string size) is upto 4096 characters
- Maximum number of processes(foreground+ background) at any point of time in the shell should not be gretaer than 4096 as the struct that stores processes has only 4096 items.


2.  `Foreground process`
- If various backgroud process commands are given input at one time and more than one of the process takes more than 2 seconds to exceute then the latest command which was executed will be shown in the prompt display of next prompt

3. `Log`
- The commands in the log files are stored in a way such the most recent command is at the bottom
- When done log execute n , nth recent most command will be executed
- Any command contaning log will not be enetered in the log file
- All Erronous commnands are also stored in log file

4. `Seek`
- search path is just the file/directory name not an entire path and target path can be anything relative or absolute
- Maximum paths that can be stored is upto 1000


5. `Multiple commands at once`
- When multiple commands are given in a single seperated by ; then a command will execute only after the previous command has completed its execution execpt when the previous command is background command it will execute regardless. Execution happens ina sequential order.

6. `Invalid Commands`
- Invalid commands when eneterd it will print
   ```
   Invalid Command
   ```
    or 
   ```
   execvp error: No such file or directory 
    ```


7. `Proclore`
- In this the units of virtual memory are Kilo Bytes as Memory obtained form ```/proc/<pid>/status ```
is indeed in kilobytes (KB)

8. `Exit`
-  Enter `exit` command to come out of shell
or press `Ctrl + D` on keyboard.


9.  **Multiple Commands in Argument:** Commands separated by `;` or `|` in a single argument will be executed sequentially or piped, respectively.

10. **Foreground Commands:** For multiple foreground commands, only the most recent command that runs for more than 2 seconds will be displayed in the next prompt. For example:
   - `sleep 5 ; echo "hello"`: THe next prompt will not show anything as the last executed command `echo "hello"` did not run for more than 2 seconds
   - `sleep 5 ; sleep 3`: The next prompt will show `sleep 3` as it runs for more than 2 seconds and it is the lastest command executed.

11. **Background Processes:** Each background process will be executed one by one. Multiple `sleep` commands are not allowed in a single background command.

12. **`hop` Command:** The `hop` command will navigate to the latest directory visited before the current one.

13. **Alias in `myshrc`:** An alias is valid only if it starts with the keyword `alias`.

14. **IO Redirection Paths:** File paths in input and output redirection must be absolute.

15. **Error Message for Same Input and Output Files:** When the input and output files are the same, the message "cat: a.txt: input file is output file" will be displayed.
16. **echo** : The `echo` command will give output considering the `"` at begining and end as part of input . for ex: if the output for `echo "hello world!"` will be `"hello world!"`  and `echo hello world!` would be `hello world!`
17. The above **echo** conditon also effects the **wc** command as it counts `"` also.
18. In **activities** the states which can be shown are : 
```
    switch (state_char) {
        case 'R': return "Running";
        case 'S': return "Running";
        case 'T': return "Stopped";
        case 'Z': return "Zombie";
        default: return "Unknown";
    }
``` 
* Here , I have included more states than what was asked (`Stopped` and `Running`) for more clarity. 
* Also, `Sleeping` is shown as runnig as **sleep** command will be a running command and it is **not stopped**.
19. **Ctrl + D** : will kill all the commands and prints as :
```
123661 : Process Killed 
Logging out...
Bye!
```
