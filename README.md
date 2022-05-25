# League-Auto-Accept
Match auto acceptor for League of Legends on Linux

Made to be used with the lutris script https://lutris.net/games/league-of-legends/ as it uses it to check the location of the league of legends installation. Set this program as the prelaunch script.


The program tries to set `abi.vsyscall32=0` to appease the league anticheat by default. You can tell it not to by using the `--nopkexec` as the first argument. i.e. `autoaccept --nonpkexec`
