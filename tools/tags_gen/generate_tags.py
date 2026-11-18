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
    Path(header_path).parent.mkdir(parents=False, exist_ok = True)
    Path(source_path).parent.mkdir(parents=False, exist_ok = True)

    for path in json_paths:
        print(f'\treading: "{os.path.abspath(path)}"')
        if os.path.exists(path):
            with open(path, 'r') as f:
                all_tags.extend(json.load(f))
        else:
            print("json file not found")

    print('\n\n')

    with open(header_path, 'w') as h:
        h.write("// GENERATED CODE - DO NOT EDIT\n// ReSharper disable CppUnusedIncludeDirective\n")
        h.write("#ifndef GENERATED_TAGS_H\n")
        h.write("#define GENERATED_TAGS_H\n")
        h.write("#include <QQmlEngine>\n")
        h.write("\n")
        h.write("namespace CALUMI::Motion::Tag { \n")
        h.write("\tQ_NAMESPACE\n\tQML_ELEMENT\n\n")
        h.write("\tenum TagId : quint32 { \n")

        no_label = False
        for tag in all_tags:
            if "id" not in tag:
                print(f"\tmissing id in entry: {tag}")
                continue
            if "tag" not in tag:
                print(f"\tmissing tag in entry: {tag}")
                continue
            if "label" not in tag:
                print(f"\tmissing label in entry: {tag}")
                no_label = True

            entry_id = tag["id"]
            if entry_id in seen_ids:
                raise ValueError(f"!!! Error: Duplicate TagId found: {entry_id} !!!")
            seen_ids.add(entry_id)

            entry_tag = tag["tag"]
            if entry_tag.lower() in seen_tags:
                raise ValueError(f"!!! Error: Duplicate Tag String found: {entry_tag} !!!")
            seen_tags.add(entry_tag.lower())

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
                print(f"\tmissing id in entry: {tag}")
                continue
            if "tag" not in tag:
                print(f"\tmissing tag in entry: {tag}")
                continue
            if "label" not in tag:
                print(f"\tmissing label in entry: {tag}")

            entry_id = entry["id"]
            entry_tag = entry["tag"].lower()
            entry_label = entry.get("label", "")

            c.write(f"\t\t\t{{ \"{entry_tag}\" , Tag::{entry_id} }}, //{entry_label}\n")
        c.write("\t};\n\n")

        c.write("\tstatic QList<QPair<QString, QString>> s_tagIds = {\n")
        for entry in all_tags:
            if "id" not in tag:
                print(f"\tmissing id in entry: {tag}")
                continue
            if "tag" not in tag:
                print(f"\tmissing tag in entry: {tag}")
                continue
            if "label" not in tag:
                print(f"\tmissing label in entry: {tag}")

            entry_id = entry["id"]
            entry_tag = entry["tag"]
            entry_label = entry.get("label", "")

            if entry_label != "":
                entry_label = f"TagDictionary::tr(\"{entry_label}\")"
            else:
                entry_label = f"\"{entry_label}\""

            c.write(f"\t\t\t{{ \"{entry_tag}\" , {entry_label} }}, //{entry_id} \n")
        c.write("\t};\n\n")


        c.write("\tTagId TagDictionary::FetchTag(const QString& tag, const QString& label) {\n\n")
        c.write("\t\tif(const auto tid = Id(tag); tid != InvalidTagId)\n\t\t\treturn tid;\n\n")
        c.write("\t\tauto& inst = instance();\n\n")
        c.write("\t\tconst auto outputId = Tag::RuntimeTag + inst.m_runtimeIterator++;\n\n")
        c.write("\t\ts_tagIdHash.insert(tag.toLower(), static_cast<TagId>(outputId));\n")
        c.write("\t\ts_tagIds.append({tag, label});\n\n")
        c.write("\t\treturn static_cast<TagId>(outputId);\n")
        c.write("\t}\n\n")

        c.write("\tQString TagDictionary::Tag(const TagId id) {\n\n")
        c.write("\t\tif(s_tagIds.size() >= id)\n\t\t\treturn {};\n\n")
        c.write("\t\treturn s_tagIds.at(id).first;\n")
        c.write("\t}\n\n")

        c.write("\tQString TagDictionary::Label(const TagId id) {\n\n")
        c.write("\t\tif(s_tagIds.size() >= id)\n\t\t\treturn {};\n\n")
        c.write("\t\treturn s_tagIds.at(id).second;\n")
        c.write("\t}\n\n")

        c.write("\tTagId TagDictionary::Id(const QString& tag) {\n\n")
        c.write("\t\tif(s_tagIdHash.contains(tag.toLower())) \n\t\t\treturn s_tagIdHash.value(tag.toLower());\n\n")
        c.write("\t\treturn InvalidTagId;\n")
        c.write("\t}\n\n")

        c.write("\n} //end CALUMI::Motion \n")



if __name__ == "__main__":
    out_hii = sys.argv[1]
    out_cii = sys.argv[2]
    in_json = sys.argv[3:]
    print("starting TagId generation")
    generate_source_table(in_json, out_hii, out_cii)