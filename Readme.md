## MNAM Project

### Manuel Utilisateur

Pour utiliser l'application, il faut d'abord compiler le code avec la commande `make`, et ensuite lancer le fichier avec `./bin/main`.

Le programme vous demandera ensuite la taille de la grille, en abscisse puis en ordonnée, et une probabilité de cases **Danger/Obstacle**.
- Vous pouvez totalement charger une carte depuis un fichier avec simplement le nom du fichier en argument : `./bin/main map_example.txt`.
Le format d'une carte dans un fichier comporte d'abord sa taille en abscisse et en ordonnée, puis la carte elle-même affichée comme pour le mode manuel.
Pour plus de compréhension, voir [l'exemple](https://github.com/YazZHh/mnam-project/blob/main/map_example.txt).

Enfin, on vous demandera quel mode vous souhaitez utiliser _(manuel, automatique ou exhaustif)_ et par là suite on peut se laisser guider par ledit programme.

#### Mode automatique

Pour utiliser le mode automatique, il suffit de créer un fichier avec à chaque ligne une instruction pour le drone _(de 1 à 8 pour chaque opérations différentes que le drone peut réaliser, voir ci-dessous)_.

Pour l'instruction 2 (Move), il faut en plus rajouter la case vers laquelle on souhaite se diriger, celle-ci est indiquée par ses coordonnées, en format `CSV` sur la même ligne que l'instruction.

Pour plus de compréhension, voir [l'exemple](https://github.com/YazZHh/mnam-project/blob/main/instructions_example.txt)

liste des instructions disponibles :

|1|2|3|4|5|6|7|8|
|-|-|-|-|-|-|-|-|
|takeoff|move|avoid|return home|dock|undock|charge step|emergency stop|

### Manuel Développeur

Ce projet est conçu pour générer les données nécessaires à un algorithme d'apprentissage par renforcement (Reinforcement Learning), spécifiquement pour résoudre un MDP (Markov Decision Process).

Le mode 3 se focalise sur l'exploration exhaustive de l'espace d'états. Il génère un fichier de transitions structuré comme suit :

`ID_Source ; Action ; X_Dest ; Y_Dest ; ID_Suivant ; Reward ; Probabilité`

### WIP