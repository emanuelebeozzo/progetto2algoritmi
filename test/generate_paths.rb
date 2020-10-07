require 'rmagick'
require 'fileutils'

include Magick

# Useful constants
CELL_WIDTH = 60
PADDING = 15
CELL_HEIGHT = 50
RADIUS = 10
IMG_DIR = 'img'

# function that read the input file and returns
#  n -> number of rows
#  m -> number of columns
#  whites -> coordinates of the white points
#  blacks -> coordinates of the blacks points
def read_input_args(input)
  rows = []
  whites = []
  blacks = []
  counter = 1
   
  # reads the whole file and splits it by newlines, then split each line into its integer part. The result is appended to the rows array
  File.readlines(input).map do |line|
    rows << line.split.map(&:to_i)
  end
    
  n, m, b, w = rows[0]
    
  # add every black dot into the array
  1.upto(b) do 
    x, y = rows[counter]
    blacks << [x, y]
    counter += 1
  end
    
  # add every white dot into the array
  1.upto(w) do
    x, y = rows[counter]
    whites << [x, y]
    counter += 1
  end
    
  return n, m, blacks, whites
end

# function that draws the matrix and the dots
#   returns the background image for each paths
def draw_table(table_attributes)
  n, m, blacks, whites = table_attributes
    
  background = Image.new(m*CELL_WIDTH + 2*PADDING, n*CELL_HEIGHT + 2*PADDING) { self.background_color = "white" }
    
  # draw the horizontal lines of the matrix
  0.upto(n) do |i|
    draw_line(PADDING,PADDING+CELL_HEIGHT*i, PADDING+CELL_WIDTH*m, PADDING+CELL_HEIGHT*i, background)
  end
    
  # draw the vertical lines of the matrix
  0.upto(m) do |i|
    draw_line(PADDING+CELL_WIDTH*i,PADDING, PADDING+CELL_WIDTH*i,PADDING+CELL_HEIGHT*n, background)
  end
    
  #draw the white dots
  whites.each do |white|
    x, y = white
    draw_dot(PADDING + y*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + CELL_HEIGHT*x + CELL_HEIGHT/2.0, RADIUS, 'white', background)
  end
    
  #draw the black dots
  blacks.each do |black|
    x, y = black
    draw_dot(PADDING + y*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + CELL_HEIGHT*x + CELL_HEIGHT/2.0, RADIUS, 'black', background)
  end
    
  return background
end

# function that draw a dot and add it to the image
def draw_dot(center_x, center_y, radius, color, image)
  circle = Magick::Draw.new
  circle.stroke_linecap('round')
  circle.stroke_linejoin('round')
  circle.stroke('black')
  circle.fill(color)
  circle.ellipse(center_x, center_y, radius, radius, 0, 360)
  circle.draw(image)
end

# function that draw a line and add it to the image
def draw_line(start_x, start_y, end_x, end_y, image)
  line = Magick::Draw.new
  line.polyline(start_x, start_y, end_x, end_y)
  line.draw(image)
end

# function return paths to draw
def read_paths(output_file)
  rows = []
  paths = []
  File.readlines(output_file).map do |line|
    rows << line.chomp.split("#")
  end
  for r in rows 
    str = r[0].split(" ")
    paths << [ str[2].to_i, str[3].to_i, str[4] ]
  end
  return paths
end

def draw_path(img, path, index)
  x, y, directions = path
  # starting point
  draw_dot(PADDING + y*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + CELL_HEIGHT*x + CELL_HEIGHT/2.0, RADIUS/2.0, 'green', img)
  first_point = [x, y]
  second_point = []
  directions.each_char { |dir| 
    case dir
      when 'U'
        second_point[0] = first_point[0] - 1
        second_point[1] = first_point[1]
      when 'D'
        second_point[0] = first_point[0] + 1
        second_point[1] = first_point[1]
      when 'L'
        second_point[0] = first_point[0]
        second_point[1] = first_point[1] - 1
      when 'R'
        second_point[0] = first_point[0] 
        second_point[1] = first_point[1] + 1
      else
        raise "Unable to understand a direction in the path number #{index} of the output file"
    end

    # line
    draw_line(PADDING + first_point[1]*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + first_point[0]*CELL_HEIGHT + CELL_HEIGHT/2.0, PADDING + second_point[1]*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + second_point[0]*CELL_HEIGHT + CELL_HEIGHT/2.0, img)
        
    first_point[0] = second_point[0] 
    first_point[1] = second_point[1]
  } unless directions.nil?
    
  # last cell visited
  draw_dot(PADDING + first_point[1]*CELL_WIDTH + CELL_WIDTH/2.0, PADDING + CELL_HEIGHT*first_point[0] + CELL_HEIGHT/2.0, RADIUS/2.0, 'red', img)
  img.write("#{IMG_DIR}/path_#{index}.png")
end

# main
begin
  raise "This script accepts exactly 2 arguments from command line" if ARGV.size != 2
  # reading attributes
  table_attributes = read_input_args(ARGV[0])
  paths = read_paths(ARGV[1])
  # building background and paths
  background = draw_table(table_attributes)
  #creating img folder
  FileUtils.mkdir_p IMG_DIR
  # writing paths
  paths.each_with_index do |path, i|
    draw_path(background.copy, path, i)
  end
rescue Exception => e
  puts e.message
end
