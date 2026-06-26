# Stratagem: Object-Oriented Chess Engine

A high-performance, headless chess simulation engine built from scratch in C++. This project focuses purely on low-level platform design, clean Object-Oriented Programming (OOP) inheritance, and machine-coding interview standards, deliberately avoiding generic frontend wrappers.

## 🚀 Architectural Highlights

* **Concurrent Match Orchestration:** Implements a centralized, thread-safe platform manager capable of tracking and hosting multiple simultaneous chess matches via unique timeline instances.
* **Polymorphic Rule Validation:** Leverages clean class inheritance hierarchies to isolate and execute unique mathematical movement vectors for individual chess piece variants.
* **Look-Ahead Checkmate Engine:** Incorporates robust validation pipelines that dynamically track edge-case illegal states (pins, checks, checkmates, and stalemates) via simulated state testing.
* **Decoupled Chat Systems:** Integrates the Mediator design pattern within the match lifecycle to abstract and isolate messaging channels between players.
* **Rating-Based Matchmaking:** Employs an adjustable score-tolerance strategy pipeline to dynamically match active lobby users based on skill thresholds.

## 📂 Project Structure

```text
src/
├── main.cpp                 # Application entry point & simulation loop
├── Structures.h             # Enums (Color, PieceType), Position, Move structs
├── Piece.h                  # Abstract base Piece class & individual piece interfaces
├── Piece.cpp                # Piece subclass calculations & vector definitions
├── Board.h / .cpp           # 8x8 matrix grid tracking, placement & state mutations
├── ChessRules.h / .cpp      # Checkmate, Stalemate, and look-ahead validation logic
├── Match.h / .cpp           # Active game state, timeline history, and chat mediator
└── GameManager.h / .cpp     # Central Singleton system orchestrator & lobby manager
