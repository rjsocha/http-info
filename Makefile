REGISTRY=wyga/http-info
PLATFORMS = linux/amd64,linux/arm64/v8
BUILDX = docker buildx build --provenance false --builder multiplatform-builder --platform $(PLATFORMS)
OUTPUT = --output type=image,push=true,compression=gzip,compression-level=9
TAGS = latest

all:
	$(foreach tag,$(TAGS),$(BUILDX) $(OUTPUT) -f Dockerfile -t $(REGISTRY):$(tag) --pull . ;)
