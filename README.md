#Reverse Card Search  
Quickly look up cards that are too small to read on your screen.  

#Dependecies  
eog  
SDL2_image  
xdotool  
  
#Building  
cd src  
make all
  
#Usage  
Use the gen_dataset.sh to gerate a database from your images.  
e.g.  
./gen_dataset.sh ~/Pictures/MyCards ~/ReverseCardSearch/avg_col_4 2  
  
2 means that a database with average colors per quadrant as values is generated.  

Use find_img.sh to find the image and display it  
e.g.  
./find_img.sh !/ReverseCardSearch/avg_col_4  
