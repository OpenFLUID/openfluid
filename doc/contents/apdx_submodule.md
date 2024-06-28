# Gestion directe de simulateur avec fragments de code  {#apdx_submodule}

Cette page explique comment manipuler les simulateurs contenant des fragments en ligne de commande. La partie d'ajout/suppression de fragment peut être faite depuis DevStudio.

<br>

Il s'agit ici d'une adaptation des principes de submodule dans le contexte de simulateurs. Aucune vraie spécificité, il s'agit d'opérations faisables dès que l'on possède Git et pour des contextes bien plus variés que la gestion de fragments de code dans un simulateur.

[Page Git de référence pour les commandes Submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules)


## Créer un simulateur avec fragment de code
``` sh
git submodule add https://github.com/.../MyFragment.git fragments/MyFragment
```


## Utilisation d'un simulateur contenant un fragment

### Clonage du repo Git du simulateur incluant le fragment

``` sh
git clone --recurse-submodules MY_SIMULATOR_REPOSITORY.git
```
Cette commande clonera le simulateur ainsi que tout les submodules (tous les fragments) qu'il contient.

## Modification d'un fragment de code

### Patch sur le repo Git du fragment

2 façons de faire :

- Sûre, pas de risque de confusion ou d'effet de bord : **Modifier le fragment à part**

	1. cloner le repo du fragment dans un répertoire à part  
    `git clone https://abc.org/Codefrag.git`

	2. Effectuer les modifications et commit  
    `.../Codefrag$ git add ...` puis `git commit`
		
	3. Push les changements sur le repo du fragment  
    `.../Codefrag$ git push origin main`

- Directe, périlleuse : **Modifier le code directement dans le repo du simulateur**  
	Même manipulations que celles pour la façon précédente, mais en allant dans le répertoire contenant le code du fragment récupéré lors de la récupération du code du simulateur au lieu de le cloner dans un endroit indépendant.  
	Le principe est que les modifications au niveau du fragment seront "contenues" par le repo du fragment et que le repo du simulateur pourra juste dire que le repo du fragment a changé lors d'un git status. Il faut donc faire les manipulations (add/commit/push) en se positionnant bien au niveau du dossier du fragment pour que les opérations fonctionnent correctement

### Propagation sur les simulateurs concernés
 
1. Récupérer les changements de fragments dans le repo des simulateurs impactés en utilisant la commande `submodule update`:  
    `.../Simulator$ git submodule update --recursive --remote`

2. Puis `.../Simulator$ git add` du dossier contenant le submodule, ce qui donne pour l'exemple utilisé :  
    `.../Simulator$ git add watsfar`  
		Il devrait être visible en tant que changement dans le `.../Simulator$ git status` avant l'opération.

3. Commit ces changements du repo du *simulateur* (`.../Simulator$ git commit`) pour que le repo distant 

Si on a choisi de modifier le code du fragment dans le repo du simulateur et qu'on a plusieurs simulateurs concernés, il faut se souvenir dans quel simulateur on a modifié le code pour savoir lesquels réaligner

<br>

# Facilitation par DevStudio

Certaines procédures présentées ci-dessus peuvent être déclenchées depuis DevStudio.