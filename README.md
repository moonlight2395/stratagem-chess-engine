# ♟️ STRATAGEM:  Object-Oriented Chess Engine

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B)
![OOP](https://img.shields.io/badge/OOP-Project-success)
![Design Patterns](https://img.shields.io/badge/Design%20Patterns-Factory%20%7C%20Singleton%20%7C%20Strategy%20%7C%20Mediator-orange)
![Platform](https://img.shields.io/badge/Platform-Console-lightgrey)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

> *"Chess is not about the pieces on the board — it's about the architecture behind every move."*

STRATAGEM is a fully playable, multi-featured C++ chess engine built on a foundation of **6 classical design patterns**, clean OOP principles, and a modular architecture that mirrors the layered complexity of the game itself. Every move is validated. Every check is detected. Every design decision was intentional.

---

## 🚀 Architectural Highlights

* **Concurrent Match Orchestration** — A GameManager Singleton maintains a map<string, Match*> of live sessions, routing player actions to isolated Match instances via unique match IDs, with no shared mutable state between games.
* **Polymorphic Move Generation** — An abstract Piece base class enforces a pure virtual getPossibleMoves() contract; each subclass (King, Queen, Rook, Bishop, Knight, Pawn) encodes its own movement vectors, letting the rules engine call uniform logic without type-switching.
* **Simulated Look-Ahead Validation** — wouldMoveCauseCheck() performs in-place board mutation (remove → place → evaluate → rollback) to detect pins, discovered checks, and illegal king exposures without cloning board state.
* **Decoupled Chat via Mediator** — Match implements ChatMediator; User objects hold only a ChatMediator* pointer, never a peer reference — routing all messages through the match lifecycle with zero inter-player coupling.
* **Score-Tolerance Matchmaking** — ScoreBasedMatching implements a pluggable MatchingStrategy interface, scanning the waiting queue for the closest opponent within a configurable Elo-style tolerance window.

## 📂 Project Structure

```text
stratagem/
├── main.cpp                # Entry point, menu loop, demo runners
├── Structures.h            # Enums (Color, PieceType, GameStatus), Position, Move
├── Piece.h                 # Abstract Piece base + subclass declarations (King–Pawn)
├── Piece.cpp               # getPossibleMoves() implementations per piece type
├── Board.h / Board.cpp     # 8×8 grid, piecePosition map, movePiece / findKing / display
├── ChessRules.h / .cpp     # isValidMove, wouldMoveCauseCheck, isInCheck / Checkmate / Stalemate
├── Match.h / Match.cpp     # Turn state, moveHistory, ChatMediator implementation, score updates
└── GameManager.h / .cpp    # Singleton orchestrator, activeMatches map, matchmaking queue


## 📋 Table of Contents

- [The Opening — Introduction](#the-opening--introduction)
- [The Middlegame — Architecture & Design](#the-middlegame--architecture--design)
- [The Pieces — Features](#the-pieces--features)
- [The Setup — Installation](#the-setup--installation)
- [The Playbook — Usage](#the-playbook--usage)
- [The Notation — Design Patterns](#the-notation--design-patterns)
- [The Endgame — Roadmap](#the-endgame--roadmap)
- [The Credits — Contributing](#the-credits--contributing)

---

## 🏁 The Opening — Introduction


Chess is one of the oldest and most computationally studied games in existence. STRATAGEM doesn't just implement chess — it engineers it. Built entirely in C++ with no external dependencies, the engine features:

- A **fully validated move system** with check, checkmate, and stalemate detection
- **Undo/redo** state management via move history
- A **real-time in-game chat** system powered by the Mediator pattern
- **Intelligent matchmaking** using score-based player pairing
- A **Scholar's Mate demo** to showcase 4-move checkmate detection out of the box

---

## ⚙️ The Middlegame — Architecture & Design

STRATAGEM's architecture is organized into four logical layers:

```
┌────────────────────────────────────────────┐
│              Game Manager Layer            │  ← Singleton, Matchmaking Strategy
├────────────────────────────────────────────┤
│                Match / Chat Layer          │  ← Mediator Pattern, State Machine
├────────────────────────────────────────────┤
│              Rules Engine Layer            │  ← Strategy Pattern, Check Detection
├────────────────────────────────────────────┤
│           Board / Piece Layer              │  ← Polymorphism, Factory Pattern
└────────────────────────────────────────────┘
```

<!-- DESIGN NOTE: Replace the ASCII diagram above with a polished architecture diagram SVG or PNG. Use draw.io or Excalidraw to create a four-box layered diagram with arrows showing dependencies flowing downward. Export and save as: ![Architecture Diagram](assets/architecture.png) -->

### Class Overview

| Class | Role | Pattern |
|---|---|---|
| `GameManager` | Central orchestrator, match lifecycle | Singleton |
| `Match` | Game state, turn management, chat relay | Mediator |
| `StandardChessRules` | Move validation, check/checkmate/stalemate | Strategy |
| `Piece` (+ subclasses) | Per-piece movement logic | Factory + Polymorphism |
| `Board` | 8×8 grid, piece tracking, spatial queries | — |
| `User` | Player state, score, message dispatch | Colleague (Mediator) |
| `ScoreBasedMatching` | Opponent pairing within score tolerance | Strategy |

---

## ♟️ The Pieces — Features

### Core Gameplay
- ✅ Full legal move generation for all 6 piece types (King, Queen, Rook, Bishop, Knight, Pawn)
- ✅ Pawn double-step on first move
- ✅ Diagonal capture logic for pawns
- ✅ Check detection via opponent threat scanning
- ✅ Checkmate & stalemate detection with full move exhaustion search
- ✅ Move undo with complete board state restoration
- ✅ Move history tracking

### System Features
- ✅ Score-based matchmaking (tolerance: ±100 points)
- ✅ Real-time in-game chat between players (Mediator pattern)
- ✅ Score updates on win (+40), loss (−30), and quit penalty (−70)
- ✅ Multiple concurrent active matches via `GameManager`
- ✅ Chess notation display (`e2`, `g1`, etc.)
- ✅ Interactive CLI board with rank/file labels

### Demo Modes
- ✅ Scholar's Mate walkthrough (4-move checkmate)
- ✅ Live 2-player chess session
- ✅ Game Manager multi-player matchmaking demo
- ✅ Chat system demo

---

## 💻 The Setup — Installation

### Prerequisites

- A C++ compiler supporting **C++11 or later** (`g++`, `clang++`, or MSVC)
- No external libraries required

### Build & Run

```bash
# Clone the repository
git clone https://github.com/yourusername/stratagem.git
cd stratagem

# Compile with g++
g++ -std=c++17 -O2 -o stratagem main.cpp

# Run the engine
./stratagem
```

```bash
# On Windows (MinGW)
g++ -std=c++17 -O2 -o stratagem.exe main.cpp
stratagem.exe
```

### Expected Output

```
====== STRATAGEM ======

1. Play Chess
2. Scholar's Mate Demo
3. Game Manager Demo
4. Game Chat Demo
5. Exit

Enter Choice:
```

---

## 🎮 The Playbook — Usage

### Playing a Game

```
Enter Choice: 1
Enter White Player Name: Aditya
Enter Black Player Name: Rohit

  +---+---+---+---+---+---+---+---+
  | a | b | c | d | e | f | g | h |
  +---+---+---+---+---+---+---+---+
8 |BR |BN |BB |BQ |BK |BB |BN |BR | 8
  +---+---+---+---+---+---+---+---+
...

White (Aditya) move (or type 'undo'): e2 e4
```

### Move Format

Moves are entered in **standard algebraic file-rank notation**:

| Input | Meaning |
|---|---|
| `e2 e4` | Move piece from e2 to e4 |
| `d1 h5` | Move Queen from d1 to h5 |
| `undo` | Undo the last move |

### Scholar's Mate Demo

```
Enter Choice: 2
```

Launches an automated 4-move checkmate sequence, printing the board after each move and announcing checkmate detection.



## 📐 The Notation — Design Patterns

STRATAGEM is a practical portfolio of classical software design patterns. Here's how each one appears in the code:

### 1. 🏭 Factory Pattern — `PieceFactory`
Centralizes piece creation, decoupling instantiation from usage. Adding a new piece type requires zero changes outside the factory.
```cpp
Piece* createPiece(PieceType type, Color color) {
    switch (type) {
        case KING:   return new King(color);
        case QUEEN:  return new Queen(color);
        // ...
    }
}
```

### 2. 🎭 Strategy Pattern — `ChessRules` & `MatchingStrategy`
Rules and matchmaking are injected as interchangeable strategies. Swap `StandardChessRules` for a `Chess960Rules` implementation without touching the `Match` class.
```cpp
class ChessRules {
    virtual bool isValidMove(Move move, Board* board) = 0;
    virtual bool isCheckmate(Color color, Board* board) = 0;
};
```

### 3. 🌐 Mediator Pattern — `Match` as `ChatMediator`
`Match` implements `ChatMediator`, routing messages between `User` colleagues without them holding direct references to each other.
```cpp
void sendMessage(Message* message, User* user) override {
    User* recipient = (user == whitePlayer) ? blackPlayer : whitePlayer;
    recipient->receive(message);
}
```

### 4. 👑 Singleton Pattern — `GameManager`
One `GameManager` controls all active matches globally, preventing duplicate orchestrators and providing a single point of access.
```cpp
static GameManager* getInstance() {
    if (instance == nullptr) instance = new GameManager();
    return instance;
}
```

### 5. 🔲 Polymorphism — `Piece` Abstract Class
Each piece overrides `getPossibleMoves()`, allowing the rules engine to call uniform logic on any piece without type-checking.
```cpp
virtual vector<Position> getPossibleMoves(Position currentPos, Board* board) = 0;
```

### 6. 📦 Encapsulation — `Board` & `Position`
`Board` exposes only semantic operations (`movePiece`, `findKing`, `isOccupied`). Internal grid representation is fully hidden behind the public interface.

---

## 🔭 The Endgame — Roadmap

The game isn't over — these features are queued for future development:

- [ ] **En passant** and **castling** special moves
- [ ] **Pawn promotion** to Queen/Rook/Bishop/Knight
- [ ] **AI opponent** using Minimax with Alpha-Beta pruning
- [ ] **Persistent match history** (file I/O or SQLite)
- [ ] **GUI frontend** via SDL2 or Qt
- [ ] **Network multiplayer** over TCP sockets
- [ ] **FEN string import/export** for board state serialization
- [ ] **Chess960 (Fischer Random)** rule variant via Strategy swap
- [ ] **Time controls** (blitz, rapid, classical)
- [ ] **Unit test suite** using Google Test

---

## 🤝 The Credits — Contributing

Contributions, suggestions, and forks are welcome!

```bash
# Fork → Clone → Branch → Commit → PR
git checkout -b feature/en-passant
git commit -m "feat: implement en passant capture logic"
git push origin feature/en-passant
```

Please follow the existing code style — clean headers, encapsulated state, and extend through abstraction rather than modification.

---


<p align="center">
  Built with ♟️ and C++ &nbsp;|&nbsp; No external dependencies &nbsp;|&nbsp; 6 Design Patterns
</p>

