#  Copyright © 2025-2026 Calaverah. All rights reserved.
#  License: https://www.gnu.org/licenses/lgpl-3.0.html
#  Contact: Calaverahmedia@gmail.com

import json
import sys
import os
from string import Formatter
from pathlib import Path

def generate_source_table(json_paths, header_path, source_path):
    all_tags = []
    seen_ids = set()
    seen_tags = set()
    print(' ')
    Path(header_path).parent.mkdir(parents=False, exist_ok = True)
    Path(source_path).parent.mkdir(parents=False, exist_ok = True)

    for path in json_paths:
        print(f'\t-reading: "{os.path.abspath(path)}"')
        if os.path.exists(path):
            with open(path, 'r') as f:
                all_tags.extend(json.load(f))
        else:
            print("json file not found")

    print('\n')

    with open(header_path, 'w') as h:
        h.write("// GENERATED CODE - DO NOT EDIT\n")
        h.write("#ifndef GENERATED_TAGS_H\n")
        h.write("#define GENERATED_TAGS_H\n")
        h.write("#include <qtmetamacros.h>\n")
        h.write("\n")
        h.write("namespace CALUMI::Motion::Tag { \n")
        h.write("\tQ_NAMESPACE\n\tQML_ELEMENT\n\n")
        h.write("\tenum TagId : quint32 { \n")

        no_label = False
        for tag in all_tags:
            if "id" not in tag:
                print(f"\t-missing id in entry: {tag}")
                continue
            if "tag" not in tag:
                print(f"\t-missing tag in entry: {tag}")
                continue
            if "label" not in tag:
                print(f"\t-missing label in entry: {tag}")
                no_label = True

            entry_id = tag["id"]
            if entry_id in seen_ids:
                raise ValueError(f"!!! Error: Duplicate TagId found: {entry_id} !!!")
            seen_ids.add(entry_id)

            entry_tag = tag["tag"]
            if entry_tag in seen_tags:
                raise ValueError(f"!!! Error: Duplicate Tag String found: {entry_tag} !!!")
            seen_tags.add(entry_tag)

            entry_label = tag.get("label", "")

            h.write(f"\t\t{entry_id}, ///< {entry_tag}")
            if not no_label:
                h.write(f" - {entry_label}")
            h.write(" \n")

        h.write(f"\t\tRuntimeTag \n")
        h.write("\t};\n")
        h.write("\tQ_ENUM_NS(TagId)\n")
        h.write("}\n")
        h.write("#endif")

    with open(source_path, 'w') as c:
        c.write("// GENERATED CODE - DO NOT EDIT\n")
        c.write("#include \"TagDictionary.h\"\n")

        c.write("namespace CALUMI::Motion {\n\n")

        c.write("\tstatic QHash<QString, TagId> s_tagIdHash = {\n")
        for entry in all_tags:
            if "id" not in tag:
                print(f"\t-missing id in entry: {tag}")
                continue
            if "tag" not in tag:
                print(f"\t-missing tag in entry: {tag}")
                continue
            if "label" not in tag:
                print(f"\t-missing label in entry: {tag}")

            entry_id = entry["id"]
            entry_tag = entry["tag"]
            entry_label = entry.get("label", "")

            c.write(f"\t\t\t{{ \"{entry_tag}\" , Tag::{entry_id} }}, //{entry_label}\n")
        c.write("\t};\n")

        c.write("\tTagId TagDictionary::fetchTag(const QString& tag, const QString& label) {\n")
        c.write("\t\t\n")
        c.write("\t}\n")


        c.write("} //end CALUMI::Motion \n")



if __name__ == "__main__":
    out_hii = sys.argv[1]
    out_cii = sys.argv[2]
    in_json = sys.argv[3:]
    generate_source_table(in_json, out_hii, out_cii)