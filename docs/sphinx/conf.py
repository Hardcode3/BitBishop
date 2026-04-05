from __future__ import annotations

import os
from pathlib import Path


CONF_DIR = Path(__file__).resolve().parent
REPO_ROOT = CONF_DIR.parents[1]
BUILD_ROOT = Path(
    os.environ.get("BITBISHOP_DOCS_BUILD_DIR", str(REPO_ROOT / "build" / "docs"))
).resolve()

project = "BitBishop"
author = "BitBishop contributors"
copyright = "2026, BitBishop contributors"

extensions = ["breathe"]
templates_path: list[str] = []
exclude_patterns: list[str] = []

root_doc = "index"
primary_domain = "cpp"
highlight_language = "c++"
numfig = True
cpp_id_attributes = ["CX_FN", "CX_INLINE", "CX_CONST", "CX_EXEC", "CX_VALUE"]
suppress_warnings = [
    "duplicate_declaration.cpp",
    "source_code_parser.cpp",
    "misc.highlighting_failure",
]

breathe_projects = {
    project: str(BUILD_ROOT / "doxygen" / "xml"),
}
breathe_default_project = project
breathe_default_members = ("members",)
breathe_show_include = True
breathe_separate_member_pages = False

html_theme = "sphinx_rtd_theme"
html_title = "BitBishop Documentation"
html_logo = str(REPO_ROOT / "assets" / "bitbishop_logo_fullsize.png")
html_static_path = [str(CONF_DIR / "_static")]
html_css_files = ["custom.css"]
html_show_sphinx = False
html_show_sourcelink = False
html_theme_options = {
    "navigation_depth": 4,
    "collapse_navigation": False,
    "style_external_links": True,
}
