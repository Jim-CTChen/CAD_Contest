///for one layer,x-direction
///P = pins
///N = nets(two pins nets)
///E = edges with weight(connect two pins)
///M = movable modules
///F = fixed modules
///D = demand(x,y) - supply(x,y)
///G = grids

B2B_weight (P, E) {
	W : P*P array
	find boundry pins p_r, p_l(rightmost and leftmost)
	for e(p,q) in E :
		if (p != p_r) && (q != p_l) :
			W[p][q] = 0
		else :
			W[p][q] = 2/[(num_P - 1)*|x_p - x_q|]
	return W
}

Cx_and_dx (E, M, F, W) {
	Cx : M*M array
	dx : M*1 array
	///pi(p) = m (pi: P -> M)
	for e(p,q) in E :
		if (pi(p)=i in M) && (pi(q)=j in M) :
			Cx[i][i] += W[p][q]
			Cx[j][j] += W[p][q]
			Cx[i][j] -= W[p][q]
			Cx[j][i] -= W[p][q]
		if (pi(p)=i in F) && (pi(q)=j in F) :
			continue
		else : ///one of module is fixed
			if (pi(p)=i in F) :
				Cx[j][j] += W[p][q]
				dx[j] -= W[p][q]*x_i
			if (pi(q)=j in F) :
				Cx[i][i] += W[p][q]
				dx[i] -= W[p][q]*x_j
		
}

initial_Placement (Cx, dx, M) {
	X : M*1 array
	solve : Cx*X + dx = 0
	X = [x_1, ///new position for movable modules(x-direction)
		 x_2,
		 ...,
		 x_M]
}

phi_x (G, D) {
	///V(x,y) : potential
	phi(x,y) :  gradients of V, G*G array
	phi_x : M*1 array
	for grid(x_0,y_0) in G :
		k : constant
		///V(x,y) += D(x_0,y_0)*k / [(x-x_0)^2+(y-y_0)^2]^(1/2)
		phi(x,y) += D(x_0,y_0)*k*-(x-x_0) / [(x-x_0)^2+(y-y_0)^2]^(3/2)
	phi_x = [phi(x_1,y_1),
			 phi(x_2,y_2),
			 ...		 ,
			 phi(x_M,y_M)]	
}

initial_Cx0 (M) {
	P_avg : average module pins
	P_i : pins number of module i
	W0 : M*1 array ///move force weight
	Cx0 : M*M diag array
	for i = 1 to M :
		W0[i] = (P_avg/P_i)*(1/num_M)
		Cx0[i][i] = W0[i]
}

global_placement (Cx, Cx0, phi_x, M) {
	delat_X : M*1 array
	solve : (Cx + Cx0)*delta_X = -Cx0*phi_x
	X += delta_X ///new position for movable modules(x-direction)
}
main (P, E, M, F, D, G) {
	///initial placement(closer)
	while (True) {
		W = B2B_weight (P, E)  				///create W
		Cx_and_dx (E, M, F, W) 				///create Cx, dx
		initial_Placement (Cx, dx, M) 	 	///move module
		if 20% of grids demand > supply : 	///too close
			break
	}
	///global placement(adjust)
	while all of grids demand < supply + c :///c : constant, for routing
		W = B2B_weight (P, E)  				///create W
		Cx_and_dx (E, M, F, W) 				///create Cx, dx
		phi_x (G, D)           				///create phi_x
		initial_Cx0 (M)        				///create Cx0
		global_placement (Cx, Cx0, phi_x, M)///move module
	///quality control
		
}



















































