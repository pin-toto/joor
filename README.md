# 🖥️ Joor OS

**Joor OS** is a minimal, educational 32-bit operating system built from scratch using **C** and **Assembly**. It features a simple terminal interface with basic file management commands and runs on x86 architecture via QEMU, CD/DVD, or USB.

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
- `xorriso` or `grub-mkrescue` (for ISO creation)

**For Arch Linux:**

```bash
sudo pacman -S gcc nasm make qemu xorriso

For Ubuntu/Debian:
bash

sudo apt install gcc nasm make qemu-system-x86 xorriso

Build ISO

Clone the repository and build the ISO image:
bash

git clone https://github.com/yourusername/joor-os.git
cd joor-os
make iso

This will generate os.iso in the project root.
Run with QEMU
bash

make run

Or manually:
bash

qemu-system-i386 -cdrom os.iso -vga std -m 32

Boot on Real Hardware

    Write the ISO to a USB drive using dd (be careful!):

bash

sudo dd if=os.iso of=/dev/sdX bs=4M status=progress

    Insert the USB drive and boot from it (enable legacy/BIOS boot mode).

    Or burn the ISO to a CD/DVD and boot from it.

📁 Project Structure
text

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

📜 License

This project is released under the Unlicense License. See the LICENSE file for more details.
🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.
🙏 Credits

Built with ❤️ and Assembly + C by pintoto.

Special thanks to the OSDev community and GRUB for making this possible.
📬 Contact

    GitHub: @pin-toto
