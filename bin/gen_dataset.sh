if [ $# -le 4 ]; then
	echo "Usage: gen_dataset.sh <dir_with_images> <database> <type>"
else
	./gen_paths.sh $1 > /tmp/rvims_paths
	./dataset /tmp/rvims_paths $2 $3
fi
