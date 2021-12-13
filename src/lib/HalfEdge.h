#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <iostream>
#include <sstream>
#include <chrono>
#include <array>
#include "vec.h"
#include "Color.h"

template<class V>
using Triangle = std::array<V, 3>;

using VertexIndex = int;
using HEdgeIndex = int;
using FaceIndex = int;

const VertexIndex NOVERTEX = -1;
const HEdgeIndex NOHEDGE = -1;
const FaceIndex NOFACE = -1;

template <class VertexAttributes>
class HalfEdgeMesh
{
private:
	struct VertexData{
		VertexAttributes attributes;
		HEdgeIndex hedge;
		std::vector<HEdgeIndex> hedges;
	};
	
	struct FaceData{
		HEdgeIndex hedge;
	};

	struct HalfEdgeData{
		VertexIndex origin;

		FaceIndex face;

		HEdgeIndex next;
		HEdgeIndex prev;
		HEdgeIndex oposite;
	};

	// Data containers
	std::vector<VertexData> vert_data;
	std::vector<FaceData> face_data;
	std::vector<HalfEdgeData> hedge_data;

public:
	// Default Constructor, empty mesh
	HalfEdgeMesh() = default;

	// Constructor from triangle lists
	HalfEdgeMesh(
		const std::vector<VertexAttributes>& P,
		const std::vector<unsigned int>& indices)
	{
		for (VertexAttributes v: P)
			addVertex(v);

		for(unsigned int i = 0; i+2 < indices.size(); i+=3)
			addFace(indices[i], indices[i+1], indices[i+2]);
	}

	void print() const{
		std::cout << "n verts: " << n_verts() << '\n';
		VertexIndex v = 0;
		//for(VertexData vertex: vert_data)
		//	std::cout << v++ << ": " << vertex.hedge << '\n';
		
		std::cout << "n faces: " << n_faces() << '\n';
		FaceIndex f = 0;
		for(FaceData face: face_data)
			std::cout << f++ << ": " << face.hedge << '\n';
		
		std::cout << "n edges: " << n_edges() << '\n';
		HEdgeIndex e = 0;
		for(HalfEdgeData edge: hedge_data){
			std::cout << e++ << ": " 
				<< edge.origin << ' ' 
				<< edge.face << ' ' 
				<< edge.next << ' ' 
				<< edge.prev << ' ' 
				<< '\n';
		}
	}

	// Number of elements in data containers
	int n_verts() const { return vert_data.size(); }
	int n_hedges() const { return hedge_data.size(); }
	int n_edges() const { return n_hedges()/2; }
	int n_faces() const { return face_data.size(); }

	// Vertex data access functions
	VertexAttributes vertex_attributes(VertexIndex v) const{ return vert_data.at(v).attributes; }
	HEdgeIndex vertex_hedge(VertexIndex v) const{ return vert_data.at(v).hedge; }

	// Edge data access functions
	VertexIndex origin(HEdgeIndex e) const{ return hedge_data.at(e).origin; };

	FaceIndex hedge_face(HEdgeIndex e) const{ return hedge_data.at(e).face; };

	HEdgeIndex next(HEdgeIndex e) const{ return hedge_data.at(e).next; };
	HEdgeIndex prev(HEdgeIndex e) const{ return hedge_data.at(e).prev; };
	HEdgeIndex oposite(HEdgeIndex e) const{ return hedge_data.at(e).oposite; };
	
	// Face data access functions
	HEdgeIndex face_hedge(FaceIndex f) const{ return face_data.at(f).hedge; }

	// Get the other vertex of edge
	VertexIndex other_vertex(HEdgeIndex e) const{
		return origin(oposite(e));
	}

	// Get the three vertices of f
	Triangle<VertexIndex> getTriangle(FaceIndex f) const{
		HEdgeIndex e1 = face_hedge(f);
		HEdgeIndex e2 = next(e1);
		HEdgeIndex e3 = prev(e1);

		return {origin(e1), origin(e2), origin(e3)};
	}
	
	// Get rotation of edge e around v clockwise
	HEdgeIndex rotate_right(HEdgeIndex e) const{
		return next(oposite(e));
	}

	// Get rotation of edge e around v counter-clockwise
	HEdgeIndex rotate_left(HEdgeIndex e) const{
		HEdgeIndex pe = prev(e);
		return pe != NOHEDGE? oposite(pe): NOHEDGE;
	}

	// Get the three edges of f
	Triangle<HEdgeIndex> getEdges(FaceIndex f) const{
		HEdgeIndex e = face_hedge(f);
		return {e, next(e), prev(e)};
	}

	// Get the neighbor vertices of v
	std::vector<VertexIndex> star(VertexIndex v) const{
		vector<VertexIndex> L;

		HEdgeIndex e = vertex_hedge(v);

		if(e == NOHEDGE)
			return L;

		HEdgeIndex first_e = e;
		// Rotate right
		while(e != NOHEDGE){
			L.push_back(other_vertex(e));
			e = rotate_right(e);

			// closed loop
			if(e == first_e){
				L.push_back(L.front());
				return L;
			}
		}

		// Rotate left
		e = rotate_left(first_e);
		while(e != NOHEDGE){
			L.insert(L.begin(), other_vertex(e));
			e = rotate_left(e);
		};

		return L;
	}

	std::vector<VertexAttributes> getVertices() const{ 
		std::vector<VertexAttributes> ret;
		for(VertexData vertex: vert_data)
			ret.push_back(vertex.attributes);
		return ret; 
	}

	std::vector<unsigned int> getIndices() const{
		std::vector<unsigned int> indices;
		for (FaceIndex f = 0; f < n_faces(); f++)
		{
			Triangle<VertexIndex> tri = getTriangle(f);
			indices.push_back(tri[0]);
			indices.push_back(tri[1]);
			indices.push_back(tri[2]);
		}
		return indices;
	}

	// Add vertex 
	void addVertex(VertexAttributes v){
		vert_data.push_back({v, NOHEDGE, {}});
	}

	// Add triangle
	void addFace(VertexIndex a, VertexIndex b, VertexIndex c){
		face_data.push_back({NOHEDGE});

		FaceIndex face{n_faces() - 1};

		HEdgeIndex e1 = vert_data.at(a).hedge = addEdge(a, b, face);
		HEdgeIndex e2 = vert_data.at(b).hedge = addEdge(b, c, face);
		HEdgeIndex e3 = vert_data.at(c).hedge = addEdge(c, a, face);

		face_data.at(face).hedge = e1;

		setNextPrev(e1, e2, e3);
		setNextPrev(e2, e3, e1);
		setNextPrev(e3, e1, e2);
	}

private:
	// Find edge with extremes a and b
	HEdgeIndex findEdge(VertexIndex a, VertexIndex b) const{
		for(HEdgeIndex e: vert_data.at(a).hedges)
			if( other_vertex(e) == b )
				return e;

		return NOHEDGE;
	}

	// Add new edge from a to b at face f
	HEdgeIndex addEdge(VertexIndex a, VertexIndex b, FaceIndex f){
		HEdgeIndex e = findEdge(a, b);
		
		if(e == NOHEDGE){
			HEdgeIndex e1 = hedge_data.size();
			HEdgeIndex e2 = e1+1;

			hedge_data.push_back({a, f, NOHEDGE, NOHEDGE, e2});
			hedge_data.push_back({b, NOFACE, NOHEDGE, NOHEDGE, e1});

			vert_data.at(a).hedges.push_back(e1);
			vert_data.at(b).hedges.push_back(e2);
			return e1;
		}

		if(hedge_face(e) != NOFACE)
			std::cout << "WARNING! Duplicate face\n";

		hedge_data.at(e).face = f;

		return e;
	}

	// Set the next and previous edges of e1 from face f
	void setNextPrev(HEdgeIndex e1, HEdgeIndex e2, HEdgeIndex e3){
		HalfEdgeData& hedge = hedge_data.at(e1);
		hedge.next = e2;
		hedge.prev = e3;
	}
};

#endif
