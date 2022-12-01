[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=8761369&assignment_repo_type=AssignmentRepo)
# Introducción
## Descripción caso planteado

Elegimos la aplicación de la blockchain en las criptomonedas, específicamente de Bitcoin, la criptomoneda más conocida en el mundo y para la cual fue inventada la tecnología de blockchain.

## Importancia del blockchain en el rubro de las criptomonedas

Básicamente las criptomonedas están basadas 100% bajo la tecnología de blockchain. Si no existiera el blockchain, tampoco lo harían las criptomonedas. Una blockchain permite la confirmación de transacciones sin la necesidad de una autoridad central. En cambio, usa un sistema distribuido por una red Peer-to-Peer.

# Explicación estructuras de datos del blockchain

Las estructuras de datos implementadas por nuestra cuenta fueron las siguientes:

1. Forward Linked List: Para almacenar los bloques dentro de la blockchain y guardar algunos otros datos.
2. Binary Search Tree: Para la búsqueda por rango.
3. Heaps: Para la búsqueda de máximo o mínimo.
4. Hash: Para la búsqueda exacta.
5. Trie: Para la búsqueda de patrones en atributos tipo string.

Nuestro proyecto presenta el siguiente diagrama de clases:

![image](https://user-images.githubusercontent.com/82919499/205078795-4beaa0dd-3218-402a-a1ab-6e5ca43e6dc7.png)

Consideraciones:

- La blockchain está compuesta de un conjunto de bloques.
- Cada bloque está compuesta por un conjunto de transacciones.
- Cada transaccion tiene sus propios atributos dependiendo del contexto.
- El primer bloque de la blockchain (el génesis) tiene asignado un código hash previo de "0".
- La información ingresada al bloque es manual o a través de un archivo .csv.
- La capacidad de transacciones que puede ser almacenada en un bloque es de 100.
- El algortimo de ecriptación es el sha-256, pero es implementado en archivos .cpp de acuerdo a un código en un blog de internet, ya que no nos fue bien al intentar instalar OpenSSL en Windows/MacOS.
- Al insertar las transacciones en los bloques, también se insertan sus atributos en las estructuras de datos correspondientes.


   ## Estrategia para asegurar integridad de su contenido

   La clase bloque tiene un método ```proof_of_work() ``` que compara si el código hash que tiene actualmente el bloque sería el mismo si se vuelve a generar el código hash (el cual toma en cuenta la información de todos los atributos de la clase bloque).

   ## Proof of Work

   El proof of work que utilizamos es que si alguien intenta agregar un bloque, primero debe encontrar un número (nonce) tal que haga que los primeros caracteres del código hash del bloque sean 0, 00, 000, 0000, 00000, según la dificultad de inserción elegida. Si se llega a encontrar ese número, recién se puede agregar el bloque a la cadena.
   
   ## Analisis de complejidad de los metodos del Blockchain
   
   - En la insercion de las transacciones se utiliza un fowardlist para poder almacenarlas obteniendo una complejidad de O(n).  
   
   ## Tabla comparativa del Blockchain con Indices VS sin indices
   
   | Blockchain | Insert | search | 
   | --- | --- | --- |
   | Con índices | Complejidad O(n) | Complejidad O(1) |
   | Sin índices | Complejidad O(n) | Complejidad O(n) |
   
   ## Estructuras de datos usados para los diferentes criterios de busqueda
   
    1. Igual a X: Hash table
      - Como se hara una simple busqueda de valor optamos por este metodo por la rapidez al acceso a datos teniendo una complejidad en general de O(1).
         
        ![image](https://user-images.githubusercontent.com/82919499/205082437-35298f4e-f6dc-4f99-8433-89ae778110de.png)

      
    2. Entre X y Y: BST
      - Con el recorrido por rango se decidio por el BST ya que este se ordena al almacenar solo bastaria con un recorrido desde el valor inicial hasta el final que           busquemos con una complejidad de O(n).
      
        ![image](https://user-images.githubusercontent.com/82919499/205085007-e07c1ce5-04e2-4904-ac46-5c71f13244c3.png)
       
         
    3. Maximo valor de: Heap
    4. Minimo valor de: Heap
          -Para los últimos 2 casos la estructura heap es la más indicada de todas ya que la forma del maxheap y del minheap mantienen en una raiz(root) al valor mayor y menor, respectivamente. Lo que da una complejidad de O(1).
          
         ![image](https://user-images.githubusercontent.com/82919499/205087704-611831d1-5de4-43d2-a9b7-48b9461b0719.png)

    5. Inicia con:
      -Para encontrar palabras que inicien con un determinado texto ingresado por el usuario. Utilizamos un Trie (Prefix Tree) para que la complejidad de búsqueda sea en el peor caso O(m), donde m es el tamaño del texto a buscar.
      
    ![image](https://user-images.githubusercontent.com/82919499/205085721-ce8877aa-c136-412c-a690-15c8adde899c.png)

      
    6. Está contenido en:
      -También utilizamos el Trie para almacenar las palabras. Se haría una búsqueda lineal de las palabras y por cada palabra aplicaría el algoritmo de Booyer Moore para encontrar el patrón especificado en la lista de palabras.
      
     ![image](https://user-images.githubusercontent.com/82919499/205086354-78bc0f1b-d9f7-4944-beb9-a2f734dc14d2.png)


   ## Dataset utilizado

   El dataset utilizado como ejemplo para la implementación fue proporcionada por el profesor.
   
# Conclusion

   En conclucion la Blockchain es un conjunto de bloques que permiten almacenar un conjunto de transacciones. Todo esto asegurado gracias a el algortimo de ecriptación sha-256. En la recreacion de este algoritmo nosotros pudimos enlazar los bloques gracias al fowardlist y ademas crear un manejo de la blockchain por medio de comandos en la terminal que permite al usuario realizar diferentes operaciones. Ademas, para poder ver las diferentes transacciones se lograron realizar diferentes tipos de busqueda aplicando diferentes estructuras de datos(hash, BST, Heap) buscando la mayor eficiencia posible en cada uno. Finalmente, podemos notar como esta implementación es algo compleja y el tiempo de respuesta aun es de 0ms solo con el uso de unos pocos datos y que al momento que estos van aumentando la misma complejidad va incrementando al igual q el mismo tiempo de respuesta.


# Listas actividades para la parte 1

[Actividades](https://github.com/users/Neo-Zapata/projects/4/views/1)

# Referencias Bibliográficas

1. https://www.pwc.com/us/en/industries/financial-services/fintech/bitcoin-blockchain-cryptocurrency.html
2. https://andersbrownworth.com/blockchain/
3. https://bitcoin.org/bitcoin.pdf
4. https://www.euromoney.com/learning/blockchain-explained/how-transactions-get-into-the-blockchain
