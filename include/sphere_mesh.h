#include "MeshBuffers.h"

inline MeshBuffers sphere_mesh(int m, int n){
	int N = m*n;

	float u0 = 0, u1 = M_PI, du = (u1-u0)/(m-1),
	      v0 = 0, v1 = 2*M_PI, dv = (v1-v0)/(n-1);

	struct Vertex{
		vec3 position;
		vec2 texCoords;
		vec3 normal;
	};
	vector<Vertex> V(N);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			float u = u0 + i*du;
			float v = v0 + j*dv;

			size_t ij = i + j*m;
			V[ij].position = {sin(u)*cos(v), sin(u)*sin(v), cos(u)};

			float s = i/(m-1.0);
			float t = j/(n-1.0);

			V[ij].texCoords = {s, t};

			V[ij].normal = normalize(V[ij].position);
		}
	}

	vector<unsigned int> indices;
	for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			unsigned int ij = i + j*m;
			indices.insert(indices.end(), {
					ij, ij+1, ij+m,
					ij+m+1, ij+m, ij+1
					});
		}
	}

	return MeshBuffers{V, indices};
}

