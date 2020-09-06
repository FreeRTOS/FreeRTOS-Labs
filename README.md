## FreeRTOS-Labs
This repository contains libraries and demos that are fully functional, but may be experimental or undergoing optimizations and refactorization to improve memory usage, modularity, documentation, demo usability, or test coverage.  

In this repository, we provide two demo projects: 

- FAT file system demo: 

  ```
  FreeRTOS-Labs\Demo\FreeRTOS_Plus_FAT_Windows_Simulator\FreeRTOS_Plus_FAT.sln
  ```

- POSIX threading API demo:

  ```
  FreeRTOS-Labs\Demo\FreeRTOS_Plus_POSIX_with_actor_Windows_Simulator\FreeRTOS_Plus_POSIX_with_actor.sln
  ```


## Cloning this repository
This repo uses [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to bring in dependent components.

Note: If you download the ZIP file provided by GitHub UI, you will not get the contents of the submodules. (The ZIP file is also not a valid git repository)

To clone using HTTPS:
```
git clone https://github.com/FreeRTOS/FreeRTOS-Labs.git --recurse-submodules
```
Using SSH:
```
git clone git@github.com:FreeRTOS/FreeRTOS-Labs.git --recurse-submodules
```

If you have downloaded the repo without using the `--recurse-submodules` argument, you need to run:
```
git submodule update --init --recursive
```


## For official FreeRTOS releases
https://github.com/FreeRTOS/FreeRTOS


## Getting help
If you have any questions or need assistance troubleshooting your FreeRTOS project, we have an active community that can help on the [FreeRTOS Community Support Forum](https://forums.freertos.org).
