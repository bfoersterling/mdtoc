#!/usr/bin/bash

current_tag="$(git describe --tags --abbrev=0)"
major_version="$(echo "$current_tag" | cut -d'.' -f1 | cut -d'v' -f2)"
minor_version="$(echo "$current_tag" | cut -d'.' -f2)"
patch_version="$(echo "$current_tag" | cut -d'.' -f3)"

echo "Current tag: ${current_tag}"

if [[ ! "$current_tag" =~ v[0-9]+\.[0-9]+\.[0-9]+ ]]; then
	echo "Current tag does not look like a semver tag."
	exit 1
fi

echo "major_version: ${major_version}"
echo "minor_version: ${minor_version}"
echo "patch_version: ${patch_version}"

read -rp "Do you want to increment the major, minor or patch version? (major, minor, patch) "

case "$REPLY" in
major)
	((major_version++))
	;;
minor)
	((minor_version++))
	;;
patch)
	((patch_version++))
	;;
*)
	echo "Invalid value - pass major/minor/patch."
	exit 1
	;;
esac

new_version="v${major_version}.${minor_version}.${patch_version}"

echo "New version: ${new_version}"

if git tag "$new_version"; then
	echo "New tag ${new_version} was set."
else
	echo "Failed to set new tag."
	exit 1
fi

echo "Create new github release."

if ! gh release create "$new_version" --title "$new_version"; then
	echo "Failed to create new github release."
fi

make

gh release upload "$new_version" "mdtoc"

exit 0
