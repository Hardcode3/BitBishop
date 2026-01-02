---
name: 🚀 Feature / Task
about: Plan and track the development of engine features and logic.
title: "[TASK]: "
labels: feat, testing
assignees: ""
---

## 🎯 Goal

Implement **Piece Square Tables (PST)** to improve the engine's positional understanding.

## 🧠 Key Idea / Logic

Assign a bonus or penalty value to every piece based on the square it occupies.

- **Score:** $Material\_Value + PST[piece][square]$
- Example: A Knight on `d4` gets a $+20$ bonus, while a Knight on `a1` gets a $-50$ penalty.

## 🛠️ Tasks

- [ ] Define $8 \times 8$ arrays for each piece type (Pawn, Knight, Bishop, etc.).
- [ ] Implement a function to flip the table for the Black pieces.
- [ ] Integrate PST values into the main `evaluate()` function.
- [ ] Ensure the score updates dynamically during the search.

## ✅ Acceptance Criteria

- [ ] The engine prefers developing pieces toward the center in the opening.
- [ ] Evaluation score is symmetrical (White's advantage at a position is the same as Black's if the board is mirrored).
- [ ] No performance regression in the search speed.

## 📝 Notes

Keep values small initially to avoid the engine becoming too "obsessed" with specific squares at the cost of material.
