import cv2
import shutil


# Read the output file cinng vertices and edges
with open("Feature_Matching_DT\Delaunay_Triangulation\output_vertices_edges_b.txt", "r") as file:
    lines = file.readlines()

base_image = cv2.imread("Feature_Matching_DT\Fast_Detection\input2.jpeg")  # Replace with your existing image
height, width = base_image.shape[:2]
# Draw edges on the existing image
# Parse vertices and edges data
vertices = []
edges = []
for line in lines:
    if line.startswith("Vertex"):
        vertex_str = line.split("(")[1].split(")")[0]
        x, y, z = map(float, vertex_str.split(","))
        vertices.append((int(x*width/10), int(y*height/10)))  # Assuming 2D po fr image display
    elif line.startswith("Edge"):
        start_index = line.index("[")
        end_index = line.index("]")

        # Extract the substring containing the coordinates
        coordinates_line = line[start_index:end_index + 1]
        print(coordinates_line)
        # Remove parentheses and split the coordinates into individual values
        coords = coordinates_line.replace('[', '').replace(']', '').split(', ')

        # Extracting individual coordinates
        x1, y1, z1, x2, y2, z2 = map(float, coords)
        edges.append(((int(x1*width/10), int(y1*height/10)), (int(x2*width/10), int(y2*height/10))))  # Assuming 2D po fr image display

copied_image = base_image.copy()

# Draw edges on the copied image
for edge in edges:
    cv2.line(copied_image, edge[0], edge[1], (0, 255, 0), 1)  # Green lines, adjust thickness if needed
for vertex in vertices:
    cv2.circle(copied_image, vertex, 3, (255, 0, 0), -1)
# Save the copied image with the triangulation
shutil.copyfile("Feature_Matching_DT\Fast_Detection\input2.jpeg", "Feature_Matching_DT\Fast_Detection\output2.jpeg")  # Make a copy of the original image
cv2.imwrite("Feature_Matching_DT\Fast_Detection\output2.jpeg", copied_image)  # Save the modified image


# Display the copied image with the triangulation
cv2.imshow("Delaunay Triangulation", copied_image)

with open(r"Feature_Matching_DT\Delaunay_Triangulation\final_features.txt", "r") as file2:  # Replace with your second vertices file
    lines2 = file2.readlines()

# Parse the second set of vertices data
vertices2 = []
for line in lines2:
    if line.startswith("Vertex"):
        vertex_str = line.split("(")[1].split(")")[0]
        x, y, z = map(float, vertex_str.split(","))
        vertices2.append((int(x*width/10), int(y*height/10)))  # Assuming 2D points for image display

# Create another copy of the image for drawing the second set of vertices
copied_image2 = base_image.copy()

# Draw vertices from the second set on the second copied image
for vertex in vertices2:
    cv2.circle(copied_image2, vertex, 3, (255, 0, 0), -1)  # Blue circles for second set of vertices
shutil.copyfile("Feature_Matching_DT\Fast_Detection\input1.jpeg", r"Feature_Matching_DT\Fast_Detection\features.jpeg")  # Make a copy of the original image
cv2.imwrite(r"Feature_Matching_DT\Fast_Detection\features.jpeg", copied_image2) 

cv2.imshow("Features final", copied_image2)
cv2.waitKey(0)
cv2.destroyAllWindows()