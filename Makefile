V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk
include tiny3d/t3d.mk
N64_CXXFLAGS = -std=c++17 -std=gnu++20 -fpermissive
srcFiles = $(wildcard $(SOURCE_DIR)/*/*.cpp) $(wildcard $(SOURCE_DIR)/*.cpp)

OBJS = $(srcFiles:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)

assets_logos	= $(wildcard assets/logos/*.png)
assets_sprites	= $(wildcard assets/sprites/*.png)
assets_sprites_RGBA32 = $(wildcard assets/sprites/RGBA32/*.png)
assets_fonts	= $(wildcard assets/fonts/*.ttf)
assets_gltf		= $(wildcard assets/models/*.glb)

assets_conv		= $(addprefix filesystem/,$(notdir $(assets_logos:%.png=%.sprite))) \
				  $(addprefix filesystem/sprites/,$(notdir $(assets_sprites:%.png=%.sprite))) \
				  $(addprefix filesystem/sprites/RGBA32/,$(notdir $(assets_sprites_RGBA32:%.png=%.sprite))) \
				  $(addprefix filesystem/,$(notdir $(assets_fonts:%.ttf=%.font64))) \
				  $(addprefix filesystem/,$(notdir $(assets_gltf:%.glb=%.t3dm))) \
				  $(wildcard filesystem/*.bin)

all: stargard.z64

MKSPRITE_LOGO_FLAGS=#--format RGBA16
filesystem/%.sprite: assets/logos/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_LOGO_FLAGS) -o filesystem "$<"

MKSPRITE_CI4_FLAGS=--format CI4 --verbose
filesystem/sprites/%.sprite: assets/sprites/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE CI4] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_CI4_FLAGS) -o filesystem/sprites "$<"

MKSPRITE_RGBA32_FLAGS=--format RGBA32 --verbose
filesystem/sprites/RGBA32/%.sprite: assets/sprites/RGBA32/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE RGBA32] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_RGBA32_FLAGS) -o filesystem/sprites/RGBA32 "$<"

filesystem/%.font64: assets/fonts/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/PixelFraktur.font64:	MKFONT_FLAGS+=--size 36
filesystem/Tannenberg.font64:	MKFONT_FLAGS+=--size 20
filesystem/FreePixel.font64:	MKFONT_FLAGS+=--size 12
filesystem/OwreKynge.font64:	MKFONT_FLAGS+=--size 16
filesystem/BlackForest.font64:	MKFONT_FLAGS+=--size 14

filesystem/%.t3dm: assets/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [T3D-MODEL] $@"
	$(T3D_GLTF_TO_3D) "$<" $@
	$(N64_BINDIR)/mkasset -c 2 -o filesystem $@

filesystem/%.bin: assets/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [COLL] $@"
	tools/save_triangles/save_triangles $< $@

$(BUILD_DIR)/stargard.dfs: $(assets_conv)
$(BUILD_DIR)/stargard.elf: $(OBJS)

stargard.z64: N64_ROM_TITLE="Stargard"
stargard.z64: $(BUILD_DIR)/stargard.dfs


clean:
	rm -rf $(BUILD_DIR)/* *.z64


-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean