# SolOS

SolOS is a open-source 32-bit operating system written in C and x86 assembly.

## Getting started
### Running the OS
1. **Clean** the system using 
```bash
make clean
```
2. **Build** the OS using 
```bash
make
```
3. **Run in QEMU** using either 
```bash
qemu-system-i386 -drive format=raw,file=build/solos.img,if=floppy -boot a
```
 or
```bash
make run
```

---
### Contributing
If you plan on contributing to the project **please read** [for contributors](CONTRIBUTING.md)

---

### License
SolOS is licensed under **GNU GPL v2**. See the [LICENSE](LICENSE.md) file for full license text.