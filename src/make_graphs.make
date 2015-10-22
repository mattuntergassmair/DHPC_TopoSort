GRAPH_DIR := graph_output
GRAPHSRC_FILES := $(wildcard $(GRAPH_DIR)/*.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)

viz: $(GRAPHIMG_FILES)
	display $(GRAPH_DIR)/*.png;

%.png: %.gv
	# circo -Tpng $< -o $@
	dot -Tpng $< -o $@

