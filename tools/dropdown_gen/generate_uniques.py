#  Copyright © 2025-2026 Calaverah. All rights reserved.
#  License: https://www.gnu.org/licenses/lgpl-3.0.html
#  Contact: Calaverahmedia@gmail.com

import json
import sys
import os
from string import Formatter
from pathlib import Path

def generate_source_table(json_paths, header_path, source_path):

    all_dropdowns = []
    seen_ids = set()
    seen_dropdowns = []
    seen_labels = []
    Path(header_path).parent.mkdir(parents=False, exist_ok = True)
    Path(source_path).parent.mkdir(parents=False, exist_ok = True)

    for path in json_paths:
        print(f'\treading: "{os.path.abspath(path)}"')
        if os.path.exists(path):
            with open(path, 'r') as f:
                all_dropdowns.extend(json.load(f))
        else:
            print("json file not found")

    print('\n\n')

    with open(header_path, 'w') as h:
        h.write("// GENERATED CODE - DO NOT EDIT\n// ReSharper disable CppUnusedIncludeDirective\n")
        h.write("#ifndef GENERATED_UNIQUE_DROPDOWNS_H\n")
        h.write("#define GENERATED_UNIQUE_DROPDOWNS_H\n")
        h.write("#include <QQmlEngine>\n")
        h.write("\n")
        h.write("namespace CALUMI::Motion::Dropdown { \n")
        h.write("\tQ_NAMESPACE\n\tQML_ELEMENT\n\n")
        h.write("\tenum DropId : quint32 { \n")

        for drop in all_dropdowns:
            if "id" not in drop:
                print(f"\tmissing id in entry: {drop}")
                continue
            if "list" not in drop:
                print(f"\tmissing list in entry: {drop}")

            entry_id = drop["id"]
            if entry_id in seen_ids:
                raise ValueError(f"!!! Error: Duplicate DropId found: {entry_id} !!!")
            seen_ids.add(entry_id)

            entry_list = [drop.get("list", []), drop.get("labels", [])]
            seen_dropdowns.append(entry_list)

            h.write(f"\t\t{entry_id}, ///< {entry_list}")
            h.write(" \n")

        h.write(f"\t\tDropIdCount \n")
        h.write("\t};\n")
        h.write("\tQ_ENUM_NS(DropId)\n")
        h.write("}\n")
        h.write("#endif")

    with open(source_path, 'w') as c:
        for entry in seen_dropdowns:
            c.write("{\"")
            for str in entry[0]:
                c.write(f"{str}\0")
            c.write("\", DropdownCollection::tr(\"")
            for idx, str in enumerate(entry[0]):
                if 0 <= idx < len(entry[1]):
                    tr_str = entry[1][idx]
                    if tr_str != "":
                        c.write(f"{entry[1][idx]}\0")
                    else:
                        c.write(f"{str}\0")
                else:
                    c.write(f"{str}\0")
            c.write("\")},\n")


if __name__ == "__main__":
    out_hii = sys.argv[1]
    out_cii = sys.argv[2]
    in_json = sys.argv[3:]
    print("starting unique dropdown generation")
    generate_source_table(in_json, out_hii, out_cii)