
#This failed absolutely 

 
def compute_accumulator_array(target_I, r_table):

    edges = canny(target_I) 
    orientations = grad_directions(edges) 
    # -------------------------------
    # Implement your code here
    # -------------------------------
    
    
    accumulator = np.zeros(target_I.shape)

    # Each pixel of acc
        # Each element of r_table
            # if ϕ(r,α ) = (xₑ, yₑ ) - (Δ x, Δ y)(r, α)  
    # Go over each pixel (x_e, y_e) and find out if 
    for xe in range(target_I.shape[0]):
        for ye in range(target_I.shape[1]):
            centerPos = np.array( [xe,ye])
            for phi_ind, phi_list in enumerate(r_table):
                for el in phi_list: 
                    test_pixel= np.uint32( centerPos +polar_to_cartesian(el))
                    print(test_pixel)
                    angle_to_index(orientations[ test_pixel[0], test_pixel[1]], r_table)
                    if phi_ind == angle_to_index(orientations[ test_pixel[0], test_pixel[1]], r_table):
                        accumulator[xe,ye] +=1
    
    
    # -------------------------------
    return accumulator
