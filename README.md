création structure de données pour maillage 3D.

* Maillage défini par :
	- liste de triangles
	- liste de sommets
* Triangle défini par :
	- 3 accès aux sommets (sens trigo) [indices dans liste sommets du maillage]
	- 3 accès aux tris (tri i en face du sommet i) [indices dans liste triangles du maillage]
* Sommet défini par :
	- position x, y, z
	- accès à 1 seul tri [indice]

