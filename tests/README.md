# Test Suite

## What
The Test Suite validates the core non-visual correctness of the document model, geometric hit-testing algorithms, and undo/redo history operations.

## Why
- **What each file tests:**
  - `tst_math.cpp`: Validates geometric math primitives, point-to-segment distances, stroke paddings, hit thresholds, smooth curve generation (Catmull-Rom), and gesture drag threshold logic.
  - `tst_document.cpp`: Validates `DrawingDocument` container operations, object count integrity, index lookups, and change notification signals.
  - `tst_hittesting.cpp`: Validates geometric intersection routines against circles for `FreehandStroke`, `LineObject`, `RectangleObject`, and `EllipseObject` (including filled vs. outline-only behaviors).
  - `tst_history.cpp`: Validates `AddObjectCommand` and `RemoveObjectsCommand`, ensuring strict descending-order deletion on redo and ascending-order restoration on undo, as well as undo-stack branch discard behavior.
- **What is intentionally not tested here:**
  - Visual pixel rendering and GPU shaders (these are tested via manual interactive session and QML visual checks).
  - OS-level mouse event synthesizers and window system driver quirks.

## How
- Tests are built with CMake and run via `ctest` or individual binary execution:
  ```bash
  cmake -B build -DBUILD_TESTS=ON
  cmake --build build
  ctest --test-dir build --output-on-failure
  ```
- Or run in Docker without display forwarding:
  ```bash
  docker compose run --rm test
  # or
  docker build --target builder -t qdraw:test .
  docker run --rm qdraw:test ctest --test-dir build --output-on-failure
  ```

## Files

| File | Role |
|---|---|
| `CMakeLists.txt` | CMake definitions registering `tst_math`, `tst_document`, `tst_hittesting`, and `tst_history` executables. |
| `tst_math.cpp` | Unit tests for geometry primitives, stroke padding, hit thresholds, and spline curve math. |
| `tst_document.cpp` | Unit tests for document addition, deletion, insertion, and signals. |
| `tst_hittesting.cpp` | Unit tests for spatial hit testing across all geometry shapes. |
| `tst_history.cpp` | Unit tests for undo/redo commands, index order correctness, and stacking preservation. |

## Boundaries
- Tests do **NOT** spawn GUI windows or require an X11/Wayland display server; they run completely headless in automated CI/CD environments.
