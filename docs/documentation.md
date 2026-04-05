# Documentation Build Guide

This repository can generate a static documentation site by combining:

- **Doxygen** for C++ parsing and repository Markdown ingestion
- **Sphinx + Breathe** for the final HTML site

All generated artifacts land under `build/docs/`, which makes the output easy to
publish to a `gh-pages` branch later.

## Prerequisites

- `doxygen`
- Python 3
- The Python packages listed in [`requirements.txt`](./requirements.txt)

## Install the Python dependencies

If you already use the repository virtual environment:

```bash
./venv/bin/python -m pip install -r docs/requirements.txt
```

Or with any other Python environment:

```bash
python3 -m pip install -r docs/requirements.txt
```

## Build the site

Using the repository virtual environment:

```bash
./venv/bin/python docs/build_docs.py
```

If your active shell already points to a Python environment with the required
packages installed:

```bash
python3 docs/build_docs.py
```

## Output layout

- Final site: `build/docs/html/`
- Doxygen XML: `build/docs/doxygen/xml/`
- Generated Sphinx source tree: `build/docs/source/`

The generated site entrypoint is:

```text
build/docs/html/index.html
```

That `build/docs/html/` directory is the one to publish on `gh-pages`.
