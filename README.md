# 🖥️ Joor OS

**Joor OS** is a minimal, educational 32-bit operating system built from scratch using **C** and **Assembly**. It features a simple terminal interface with basic file management commands and runs on x86 architecture via QEMU.

---

## ✨ Features

- 🚀 **Fast boot** – boots directly into the terminal
- ⌨️ **Keyboard support** – full terminal input
- 📁 **Simple file system** – in-memory file storage
- 📋 **Commands**:
  - `help` – show available commands
  - `ls` – list files
  - `rm <file>` – delete a file
  - `cout <text>` – print text to the screen
  - `clear` – clear the terminal screen
  - `info` – display system information
  - `reboot` – restart the system
  - `off` – shutdown the system

---

## 🛠️ Build & Run

### Prerequisites

Make sure you have the following tools installed:

- `gcc` (with 32-bit support)
- `nasm`
- `ld`
- `make`
- `qemu-system-x86`

**For Arch Linux:**

```bash
sudo pacman -S gcc nasm make qemu
```

**For Ubuntu/Debian:**

```bash
sudo apt install gcc nasm make qemu-system-x86
```

### Build the Kernel

Clone the repository and build the kernel:

```bash
git clone https://github.com/pin-toto/joor-os.git
cd joor-os
make
```

### Run with QEMU

```bash
make run
```

Or manually:

```bash
qemu-system-i386 -kernel build/kernel.elf -vga std -m 32
```

---

## 📁 Project Structure

```
joor/
├── boot/
│   └── grub/
│       └── grub.cfg          # GRUB boot configuration
├── src/
│   ├── include/
│   │   ├── kernel.h
│   │   ├── keyboard.h
│   │   ├── screen.h
│   │   └── shell.h
│   ├── boot.asm              # Multiboot entry point
│   ├── kernel.c              # Main kernel
│   ├── keyboard.c            # PS/2 keyboard driver
│   ├── screen.c              # VGA text mode driver
│   └── shell.c               # Simple command-line shell
├── linker.ld                 # Linker script
├── Makefile                  # Build automation
└── README.md
```

---

## 📜 License

This project is released under the **Unlicense**. See the `LICENSE` file for more details.

---

## 🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

---

## 🙏 Credits

Built with ❤️ and Assembly + C by **pintoto**.

Special thanks to the OSDev community and GRUB for making this possible.

---

## 📬 Contact

- GitHub: [@pin-toto](https://github.com/pin-toto)

---

> **Joor OS – Simple. Minimal. Yours.**
```
