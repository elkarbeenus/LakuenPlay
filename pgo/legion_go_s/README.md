# Legion Go S profiles

Raw profiles recorded on a Legion Go S (Ryzen Z2 Go) with the instrumented build (`release-binaries`, device
`legion_go_s`, PGO `generate`). Put them here as `<name>.profraw.xz` or `.profraw.zst`; the Legion Go S PGO build merges
them. Until there is one, that build uses the Steam Deck's profile in `pgo/`.

Record: start the instrumented AppImage, play the parts that matter for 15-30 minutes, quit through the menu (the profile
is written on a clean exit only) and take the newest file from `/home/deck/lichii-pgo`.
