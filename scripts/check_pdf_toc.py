"""Check rendered TOC numbers against the printed footers of their PDF targets.

Requires pypdf and Poppler's pdftotext. Reads the PDF itself, so stale .toc/.aux
files cannot make the check pass. This handbook uses Arabic page numbers and
centered footers, with a cover and TOC before the first numbered section.
"""

import argparse
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

from pypdf import PdfReader


def read_word_pages(pdf):
    result = subprocess.run(
        ["pdftotext", "-bbox-layout", str(pdf), "-"],
        check=True, capture_output=True,
    )
    # Some embedded code fonts decode to XML-forbidden control characters.
    # They cannot be digits or affect the TOC/footers being checked.
    xml = re.sub(rb"[\x00-\x08\x0b\x0c\x0e-\x1f]", b"", result.stdout)
    root = ET.fromstring(xml)
    ns = {"x": "http://www.w3.org/1999/xhtml"}
    pages = []
    for page in root.findall(".//x:page", ns):
        words = []
        for word in page.findall(".//x:word", ns):
            words.append((float(word.attrib["xMin"]),
                          float(word.attrib["yMin"]),
                          float(word.attrib["yMax"]), word.text or ""))
        pages.append((float(page.attrib["width"]),
                      float(page.attrib["height"]), words))
    return pages


def check(pdf):
    reader = PdfReader(pdf)
    pages = read_word_pages(pdf)
    if len(pages) != len(reader.pages):
        raise ValueError("PDF parsers disagree on page count")
    destinations = reader.named_destinations
    first_body = reader.get_destination_page_number(destinations["section.1"])
    if first_body is None or first_body <= 1:
        raise ValueError("Cannot locate the front matter before section.1")
    checked, errors = 0, []
    footer_cache = {}
    for index in range(1, first_body):
        width, height, words = pages[index]
        rows = {}
        for annotation in reader.pages[index].get("/Annots", []):
            annotation = annotation.get_object()
            action = annotation.get("/A", {})
            if action.get("/S") != "/GoTo":
                continue
            name = str(action.get("/D", ""))
            if not re.fullmatch(r"(?:section|subsection|subsubsection)\.[\d.]+", name):
                continue
            rect = annotation["/Rect"]
            # Convert PDF bottom-up coordinates to Poppler top-down coordinates.
            top, bottom = height - float(rect[3]), height - float(rect[1])
            old = rows.get(name, (top, bottom))
            rows[name] = (min(old[0], top), max(old[1], bottom))
        for name, (top, bottom) in rows.items():
            checked += 1
            numbers = [int(text) for x, y1, y2, text in words
                       if x > width * 0.75 and top - 2 <= (y1 + y2) / 2 <= bottom + 2
                       and re.fullmatch(r"\d+", text)]
            if len(numbers) != 1:
                errors.append(f"{name}: cannot identify one TOC page number")
                continue
            target = reader.get_destination_page_number(destinations[name])
            if target not in footer_cache:
                tw, th, target_words = pages[target]
                candidates = [(y1, int(text)) for x, y1, _, text in target_words
                              if tw * 0.4 < x < tw * 0.6 and y1 > th * 0.9
                              and re.fullmatch(r"\d+", text)]
                # Body formulas can also contain centered digits near the bottom.
                # The printed footer is on the lowest numeric baseline.
                lowest = max((y for y, _ in candidates), default=-1)
                footers = [number for y, number in candidates if y >= lowest - 2]
                if len(footers) != 1:
                    raise ValueError(f"Cannot identify the printed footer on PDF page {target + 1}")
                footer_cache[target] = footers[0]
            actual = footer_cache[target]
            if numbers[0] != actual:
                errors.append(f"{name}: TOC says {numbers[0]}, printed page is {actual} "
                              f"(PDF page {target + 1})")
    if checked == 0:
        raise ValueError("No TOC links found; refusing to pass an empty check")
    if errors:
        print(f"FAIL: {len(errors)} of {checked} TOC entries are invalid", file=sys.stderr)
        for error in errors[:12]:
            print(error, file=sys.stderr)
        return 1
    print(f"PASS: {checked} TOC entries match printed destination pages "
          f"({len(reader.pages)} PDF pages)")
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pdf", type=Path)
    args = parser.parse_args()
    sys.exit(check(args.pdf))
