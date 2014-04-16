# Embedded 1st HW `Calculator on Board with IPC`
*20091631 Minho Ryang*

## Target
- **ARM Cortex-A9 Dual Core** ([ACHRO 4210 Exynos (Samsung)](http:/huins.com/en/m12.php?m=rd&no=100), Spartan 6 FPGA)
- ARM **Cross-compiler**
- at least, 3 Processes (use `fork()`)
- at least, use IPC between Processes. (`shared-memory, message queue, pipe, ...`)
- control the `GPIO, FPGA, ...` devices by `kernel-module, mmap(), ...`
- at least, 2 Features (fully explained at [HW1.pdf](http://dcclab.sogang.ac.kr/?module=file&act=procFileDownload&file_srl=1554&sid=777df414f46f23cb456298f7c2c27506))
	- STOP WATCH
	- TEXT EDITOR
	- extra feature to more points...
+ decided by myself, I'll consider these:
	+ Good **Architecture**
	+ Good **Readability**
	+ Good **Reusability**
	+ **Modular Programming**
	+ Using [**github**](http://github.com/minhoryang/Minho-EmbeddedSystem-HW1-FPGA-IPC).
	+ **Markdown Document** (this)
	+ **Work without the device**.
		+ it means, Isolate the device related code.
	+ Exhibit my skill (Professor want it.)

## Coverage & Contents
### Coverage
- Process Management.
- IPC Management.
- Device Control for I/O
- 2 Features

### Contents
- Process Management :
	- using **fork()** for creation
	- using **SIGNAL** for extinction
- IPC Management :
	- using **Message Queue**
	- selected for **Non-blocking I/O**
- Device Control for I/O
	- I/O **Mock up**
	- **Isolation from the physical devices**
- 2 Features
	- just followed the detail usages & implement instructions @ [HW1.pdf](http://dcclab.sogang.ac.kr/?module=file&act=procFileDownload&file_srl=1554&sid=777df414f46f23cb456298f7c2c27506)
	- Isolation between the features.

## Project Timeline & Approach Method
### Project Timeline
- Deadline @ 2014-04-16 23:59
- Started to think @ 2014-03-31.
- First Design Draft @ 2014-04-10.
- Hand out @ 2014-04-16.

### Approach Method
- **Bottom up**
- I don't have much time
- To increase Productivity, **Minimize the time to use physical device**
- 1 commit == 1 feature.

## Results
- How to Build
	- `$ make` (By default, it run `make arm build`)
	- `$ make {arm|intel} {build|tests}`
		- Select Compiler: arm, intel
		- Select Build Result Set: build, tests.
    - You can get the binary at
    	- `built/20091631.out`

### Modules Diagram
- core/
	- `msgq`
	- `fork`
	- `bitmap`
	- `gameloop`
- modules/
	- `inputdev`
	- `outputdev`
	- `process`
		- `stop_watch`
		- `text_editor`

### Considerations
- Non-Blocking I/O
- Thread safety
- including Data structures or Algorithms.

### Test & Assurance Method
- tried to develop as TestDrivenDevelopment.

## etc
- Feeling?
	- I need more time to do it...
- Rooms for Development:
	- Add more testcases
	- Resolve all TODO comments
	- Add TextFile-based Input/Output module (for testing)
	- Add Terminal-based Input/Output module (with `ncurses library`).
	- Increase FPS Accuracy.
