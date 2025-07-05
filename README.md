# 🎮 Wordle Game

A terminal-based interactive **Wordle Game** written in C++, featuring a Player System, Admin Panel, colorful terminal UI, lifelines, and real-time statistics. The project demonstrates object-oriented design, file persistence, terminal controls, and bitwise operations for efficient feature management.

---
## 🕹️ Game Rules

- Guess the hidden **5-letter word** within **6 attempts**.
- After each guess:
  - 🟩 **Green Letter:** Correct letter, correct position.
  - 🟨 **Yellow Letter:** Correct letter, wrong position.
  - ⚪ **White Letter:** Letter is not in the word.
- All guesses must be valid, alphabetic 5-letter words.
- You can use **Lifelines** if available and sufficient score:
  - **Reveal Letter (100 points):** Reveals one letter in its correct position.
  - **Eliminate Letters (200 points):** Removes irrelevant letters from keyboard display.
  - **Extra Attempt (300 points):** Grants an additional guess beyond 6 tries.
- Keyboard display shows available letters after each turn.
- Final score depends on attempts remaining and correct guesses.

**Win Condition:**  
✔️ Guess the word within allowed attempts.  

**Lose Condition:**  
❌ Exhaust all attempts without guessing the word — the correct word is revealed.

---

## 📌 Features

✅ **Player Account System**  
- Secure player registration and login system  
- Tracks games played, maximum score, total score  
- Password verification logic (consider future hashing)  

✅ **Interactive Wordle Gameplay**  
- Classic 5-letter Wordle guessing game  
- Terminal-based colored feedback (Green = correct, Yellow = misplaced, White = absent)  
- Virtual keyboard shows available letters  

✅ **Lifelines System** (Resource-dependent)  
- **Reveal Letter** – Reveal one unknown letter (Cost: 100 points)  
- **Eliminate Letters** – Remove irrelevant letters from the keyboard (Cost: 200 points)  
- **Extra Attempt** – Gain one additional guess (Cost: 300 points)  
- Bitwise operations track available lifelines efficiently  

✅ **Admin Panel**  
- **Manage Users** – View all users, inspect stats, or delete accounts  
- **Manage Word List** – Add, remove, or list allowed 5-letter words  
- **View System Statistics** – See gender demographics, total games played, and top scores by gender  

✅ **Colorful Terminal UI**  
- Uses ANSI escape codes for vibrant, structured terminal output  
- Supports Windows and UNIX systems  

✅ **File Persistence**  
- Player data saved to `users.txt`  
- Word list defined within code (easy future file extension)  

---

---

## ⚙️ Technologies & Concepts

- **C++ STL:** Maps, Vectors, File Streams, Algorithms  
- **Object-Oriented Design:** Encapsulation, Modular Classes  
- **Bitwise Operations:** Efficient lifeline management  
- **Terminal UI:** ANSI escape sequences for color and layout  
- **File I/O:** User data persistence with text files  
- **Cross-Platform Compatibility:** Terminal control for Windows/Linux  

---

## 🚀 Getting Started

### Prerequisites  
- C++ Compiler (e.g., G++, Clang++)  
- Terminal that supports ANSI escape codes  

### Build & Run  

```bash
g++ -o wordle main.cpp
./wordle
