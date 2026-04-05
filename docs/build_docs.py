#!/usr/bin/env python3

from __future__ import annotations

import argparse
import importlib
import os
import shutil
import subprocess
import sys
import textwrap
import xml.etree.ElementTree as ET
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


PROJECT_NAME = "BitBishop"
REPO_ROOT = Path(__file__).resolve().parents[1]
DOCS_ROOT = REPO_ROOT / "docs"
SPHINX_CONF_DIR = DOCS_ROOT / "sphinx"
BUILD_ROOT = REPO_ROOT / "build" / "docs"
DOXYGEN_OUTPUT_DIR = BUILD_ROOT / "doxygen"
DOXYGEN_XML_DIR = DOXYGEN_OUTPUT_DIR / "xml"
SPHINX_SOURCE_DIR = BUILD_ROOT / "source"
SPHINX_DOCTREE_DIR = BUILD_ROOT / ".doctrees"

SECTION_ORDER = {
    "Repository": 0,
    "Project Guides": 1,
    "Architecture Notes": 2,
}


@dataclass(frozen=True)
class GuidePage:
    title: str
    source_path: Path
    compound_name: str
    section: str
    document_path: Path
    toctree_entry: str


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Build the BitBishop documentation site with Doxygen and Sphinx."
    )
    parser.add_argument(
        "--builder",
        default="html",
        help="Sphinx builder to use. Defaults to html.",
    )
    return parser.parse_args()


def ensure_python_dependency(module_name: str, package_name: str) -> None:
    try:
        importlib.import_module(module_name)
    except ModuleNotFoundError as error:
        message = (
            f"Missing Python dependency '{package_name}'. "
            f"Install the docs toolchain with `{sys.executable} -m pip install -r docs/requirements.txt`."
        )
        raise SystemExit(message) from error


def ensure_tooling() -> None:
    if shutil.which("doxygen") is None:
        raise SystemExit("Doxygen is required but was not found on PATH.")

    ensure_python_dependency("sphinx", "Sphinx")
    ensure_python_dependency("breathe", "breathe")
    ensure_python_dependency("sphinx_rtd_theme", "sphinx-rtd-theme")


def run(command: list[str], *, cwd: Path | None = None, env: dict[str, str] | None = None) -> None:
    printable = " ".join(command)
    print(f"$ {printable}")
    subprocess.run(command, cwd=cwd, env=env, check=True)


def reset_build_directories(builder: str) -> Path:
    builder_output_dir = BUILD_ROOT / builder

    for path in (DOXYGEN_OUTPUT_DIR, SPHINX_SOURCE_DIR, SPHINX_DOCTREE_DIR, builder_output_dir):
        if path.exists():
            shutil.rmtree(path)

    DOXYGEN_OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    SPHINX_SOURCE_DIR.mkdir(parents=True, exist_ok=True)
    return builder_output_dir


def build_doxygen_xml() -> None:
    print("[1/4] Running Doxygen")
    run(["doxygen", str(DOCS_ROOT / "Doxyfile")], cwd=DOCS_ROOT)
    copy_doxygen_assets()


def copy_doxygen_assets() -> None:
    source_assets = REPO_ROOT / "assets"
    target_assets = DOXYGEN_XML_DIR / "assets"
    if source_assets.exists():
        shutil.copytree(source_assets, target_assets, dirs_exist_ok=True)


def generate_api_pages() -> None:
    print("[2/4] Generating Breathe API pages")
    api_output_dir = SPHINX_SOURCE_DIR / "api" / "generated"
    api_output_dir.mkdir(parents=True, exist_ok=True)

    run(
        [
            sys.executable,
            "-m",
            "breathe.apidoc",
            "-f",
            "-m",
            "-o",
            str(api_output_dir),
            "-p",
            PROJECT_NAME,
            "-g",
            "class,struct,union,file,namespace",
            str(DOXYGEN_XML_DIR),
        ]
    )

    trim_generated_api_pages(api_output_dir)


def trim_generated_api_pages(api_output_dir: Path) -> None:
    # Markdown guides are surfaced separately under Guides, so keep the API
    # section focused on C++ entities and project-owned namespaces.
    for markdown_file_page in (api_output_dir / "file").glob("*_8md.rst"):
        markdown_file_page.unlink()

    std_namespace_page = api_output_dir / "namespace" / "namespacestd.rst"
    if std_namespace_page.exists():
        std_namespace_page.unlink()


def resolve_source_path(raw_path: str) -> Path:
    candidate = Path(raw_path)
    if candidate.is_absolute():
        return candidate.resolve()

    for root in (REPO_ROOT, DOCS_ROOT):
        resolved = (root / candidate).resolve()
        if resolved.exists():
            return resolved

    return (REPO_ROOT / candidate).resolve()


def classify_guide(relative_path: Path) -> str:
    if not relative_path.parts:
        return "Repository"

    if relative_path.parts[0] == "docs":
        return "Project Guides"
    if relative_path.parts[0] == "include":
        return "Architecture Notes"
    return "Repository"


def fallback_title(relative_path: Path) -> str:
    if relative_path.name == "README.md":
        return "Project Overview"
    return relative_path.stem.replace("_", " ").title()


def collect_guide_pages() -> list[GuidePage]:
    index_xml = DOXYGEN_XML_DIR / "index.xml"
    index_root = ET.parse(index_xml).getroot()

    guides: list[GuidePage] = []

    for compound in index_root.findall("compound"):
        if compound.get("kind") != "page":
            continue

        refid = compound.get("refid")
        if not refid:
            continue

        compound_xml = DOXYGEN_XML_DIR / f"{refid}.xml"
        compound_root = ET.parse(compound_xml).getroot()
        compound_def = compound_root.find("compounddef")
        if compound_def is None:
            continue

        location = compound_def.find("location")
        if location is None:
            continue

        file_name = location.get("file")
        if not file_name or not file_name.endswith(".md"):
            continue

        source_path = resolve_source_path(file_name)
        if not source_path.is_relative_to(REPO_ROOT):
            continue

        relative_path = source_path.relative_to(REPO_ROOT)
        title = (compound_def.findtext("title") or "").strip() or fallback_title(relative_path)
        compound_name = (compound_def.findtext("compoundname") or refid).strip()
        section = classify_guide(relative_path)

        document_path = SPHINX_SOURCE_DIR / "guides" / relative_path.with_suffix(".rst")
        toctree_entry = relative_path.with_suffix("").as_posix()

        guides.append(
            GuidePage(
                title=title,
                source_path=source_path,
                compound_name=compound_name,
                section=section,
                document_path=document_path,
                toctree_entry=toctree_entry,
            )
        )

    guides.sort(
        key=lambda guide: (
            SECTION_ORDER.get(guide.section, 99),
            guide.source_path.relative_to(REPO_ROOT).as_posix(),
        )
    )
    return guides


def write_file(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def build_guide_pages(guides: list[GuidePage]) -> None:
    for guide in guides:
        relative_path = guide.source_path.relative_to(REPO_ROOT).as_posix()
        underline = "=" * (len(guide.title) + 4)
        content = textwrap.dedent(
            f"""\
            {guide.title}
            {underline}

            .. rst-class:: guide-origin

               Source: ``{relative_path}``

            .. doxygenpage:: {guide.compound_name}
               :project: {PROJECT_NAME}
               :no-title:
            """
        )
        write_file(guide.document_path, content)


def build_guides_index(guides: list[GuidePage]) -> None:
    sections: dict[str, list[GuidePage]] = defaultdict(list)
    for guide in guides:
        sections[guide.section].append(guide)

    lines = [
        "Guides",
        "======",
        "",
        "Repository Markdown rendered through Doxygen and presented inside the Sphinx site.",
        "",
    ]

    for section_name in ("Repository", "Project Guides", "Architecture Notes"):
        pages = sections.get(section_name, [])
        if not pages:
            continue

        lines.extend(
            [
                section_name,
                "-" * len(section_name),
                "",
                ".. toctree::",
                "   :maxdepth: 1",
                "",
            ]
        )

        for guide in pages:
            lines.append(f"   {guide.toctree_entry}")

        lines.append("")

    write_file(SPHINX_SOURCE_DIR / "guides" / "index.rst", "\n".join(lines))


def build_root_index() -> None:
    content = textwrap.dedent(
        """\
        BitBishop Documentation
        ==========================

        This site combines the repository's narrative guides with API reference
        generated from the C++ codebase. Doxygen produces the source metadata and
        Sphinx + Breathe assemble the final site that can be published as static HTML.

        .. toctree::
           :maxdepth: 2
           :caption: Contents

           guides/index
           api/index
        """
    )
    write_file(SPHINX_SOURCE_DIR / "index.rst", content)


def build_api_index() -> None:
    content = textwrap.dedent(
        """\
        API Reference
        ===============

        The reference below is generated from the headers and implementation
        files under ``include/``, ``src/``, and ``main/``.

        .. toctree::
           :maxdepth: 2

           generated/classlist
           generated/structlist
           generated/namespacelist
           generated/filelist
        """
    )
    write_file(SPHINX_SOURCE_DIR / "api" / "index.rst", content)


def build_sphinx_source_tree() -> None:
    print("[3/4] Preparing generated Sphinx sources")
    guides = collect_guide_pages()
    build_root_index()
    build_api_index()
    build_guide_pages(guides)
    build_guides_index(guides)


def run_sphinx(builder: str, output_dir: Path) -> None:
    print("[4/4] Running Sphinx")
    env = os.environ.copy()
    env["BITBISHOP_DOCS_BUILD_DIR"] = str(BUILD_ROOT)

    run(
        [
            sys.executable,
            "-m",
            "sphinx",
            "-b",
            builder,
            "-d",
            str(SPHINX_DOCTREE_DIR),
            "-c",
            str(SPHINX_CONF_DIR),
            str(SPHINX_SOURCE_DIR),
            str(output_dir),
        ],
        env=env,
    )

    nojekyll_file = output_dir / ".nojekyll"
    nojekyll_file.write_text("", encoding="utf-8")


def main() -> int:
    args = parse_args()
    ensure_tooling()
    output_dir = reset_build_directories(args.builder)
    build_doxygen_xml()
    generate_api_pages()
    build_sphinx_source_tree()
    run_sphinx(args.builder, output_dir)

    print("")
    print(f"Documentation built successfully: {output_dir / 'index.html'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
