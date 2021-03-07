- ~~Comment on codes~~
- ~~Work on README.md~~
- ~~Work on input / output~~
- ~~Translate back from EDRRProcess to Processes~~
- Give users the option to turn on traditional priority or not
    - Perhaps also need to provide an explanation as to what traditional priority is about
- ~~For every call to calloc and malloc, check if memory address is NULL~~
- ~~Free memory after everything~~
- ~~Validate input read from file (i.e. improve `read_file()`)~~


- Test cases
- Test for starvation -- which is when you have a process waiting but all other processes arriving later are always smaller than time quantum or, basically after calculation of time quantum, waiting processes still cannot run yet because smaller processes have to go first.
