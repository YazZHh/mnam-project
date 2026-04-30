# MNAM Project

## Manuel Utilisateur

Pour utiliser l'application, il faut d'abord compiler le code avec la commande `make`, et ensuite lancer le fichier avec `./bin/main`.

Le programme vous demandera ensuite la taille de la grille, en abscisse puis en ordonnée, et une probabilité de cases **Danger/Obstacle**.
- Vous pouvez totalement charger une carte depuis un fichier avec simplement le nom du fichier en argument : `./bin/main map_example.txt`.
Le format d'une carte dans un fichier comporte d'abord sa taille en abscisse et en ordonnée, puis la carte elle-même affichée comme pour le mode manuel.
Pour plus de compréhension, voir [l'exemple](https://github.com/YazZHh/mnam-project/blob/main/map_example.txt).

Enfin, on vous demandera quel mode vous souhaitez utiliser _(manuel, automatique ou exhaustif)_ et par là suite on peut se laisser guider par ledit programme, tout y est écrit dans le terminal, toutes les entrées de l'utilisateur sont vérifiées pour éviter des erreurs.

### Mode automatique

Pour utiliser le mode automatique, il suffit de créer un fichier avec à chaque ligne une instruction pour le drone _(de 1 à 8 pour chaque opérations différentes que le drone peut réaliser, voir ci-dessous)_.

Pour l'instruction 2 (Move), il faut en plus rajouter la case vers laquelle on souhaite se diriger, celle-ci est indiquée par ses coordonnées, en format `CSV` sur la même ligne que l'instruction.

Pour plus de compréhension, voir [l'exemple](https://github.com/YazZHh/mnam-project/blob/main/instructions_example.txt)

liste des instructions disponibles :

|1|2|3|4|5|6|7|8|
|-|-|-|-|-|-|-|-|
|takeoff|move|avoid|return home|dock|undock|charge step|emergency stop|

## Manuel Développeur

Ce projet est conçu pour générer les données nécessaires à un algorithme d'apprentissage par renforcement (Reinforcement Learning), spécifiquement pour résoudre un MDP (Markov Decision Process).

Le mode 3 se focalise sur l'exploration exhaustive de l'espace d'états. Il génère un fichier de transitions structuré comme suit :

`ID_Source ; Action ; X_Dest ; Y_Dest ; ID_Suivant ; Reward ; Probabilité`

Le fichier généré (ainsi que la liste des états) permet ensuite à un développeur tiers d'implémenter un algorithme de Q-Learning. L'identifiant d'état (ID) regroupe toutes les variables nécessaires pour respecter la propriété de Markov.

## Rapport Technique

Il a été choisi, pour ce projet, de coder avec le langage `C`.

Le projet a été divisé en plusieurs fichiers, tous travaillant ensemble, piloté par le fichier `main.c`.

Dans le fichier `grille.h`, une structure `Grille` a été implémentée, elle contient la taille de la grille ainsi que son contenu grâce à un tableau de tableaux de `Case`(s), une autre structure implémentée dans le même fichier.

Le fichier `drone.h` ne contient pas de structure, en effet par souci de structuration, la structure `Drone` a dû être déplacée dans le fichier `state.h` quand le mode exhaustif a été ajouté pour **éviter les dépendances cycliques**.

C'est donc dans le fichier `state.h` que l'on retrouve la structure `Drone`, qui, de part son nom, contient toutes les états du drone (position, batterie, etc...), ainsi que la grille sur laquelle il se trouve.
Pour effecturer une action il faut donc utiliser les fonctions déclarées dans `drone.h`, seul le drone nécéssite d'être appelé.

Dans le fichier `state.h`, se trouve tout ce qui est relatif au mode exhaustif : on y retrouve la structure `State` représentant un état (structure du drone sans le pointeur vers la grille), et la structure `StateList`, une simple liste (très longue) des états qui seront calculés lors de l'exploration exhaustive.

Une touche particulière (dans le fichier `main.c`) a été apportée à la gestion d'erreur afin d'éviter que l'utilisateur n'entre de données incorrectes.
La complexité algorithmique a aussi été quelque-chose d'important durant le développement du programme mais le processus d'exploration reste quand-même peu efficace et mériterait d'être amélioré.

Bien que le processus d'exploration actuel soit gourmand en ressources, il garantit la complétude de la base de données générée, ce qui est crucial pour la convergence d'un futur algorithme d'apprentissage.

### Récompense

Pour chaque actions, le drone doit recevoir une récompense à hauteur de son action. Elle a été choisie de la manière suivante :

- Bonus d'objectif (+50) : Récompense la prise de risque pour aller explorer les zones.

- Malus de sécurité (-100) : Sanctionne sévèrement le crash ou la batterie vide, conditions d'arrêt de l'épisode.

- Bonus de réussite (+500) : Récompense beaucoup l'exploration totale (explorer les 4 zones et revenir sur la base).

- Pénalité de temps (-1) : Légère punition donnée dans le cas de base (si aucun des autres rewards n'est donné). Cela force le drone à ne pas stagner.