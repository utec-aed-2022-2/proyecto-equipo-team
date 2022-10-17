[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=8761369&assignment_repo_type=AssignmentRepo)
# Introducción
## Descripción caso planteado

Elegimos la aplicación de la blockchain en las criptomonedas, específicamente de Bitcoin, la criptomoneda más conocida en el mundo y para la cual fue inventada la tecnología de blockchain.

## Importancia del blockchain en el rubro de las criptomonedas

Básicamente las criptomonedas están basadas 100% bajo la tecnología de blockchain. Si no existiera el blockchain, tampoco lo harían las criptomonedas. Una blockchain permite la confirmación de transacciones sin la necesidad de una autoridad central. En cambio, usa un sistema distribuido por una red Peer-to-Peer.

# Explicación estructuras de datos del blockchain

Nuestro proyecto se compone de varias clases en la siguiente jerarquia:

1. Blockchain
   Atributos:
   - size: Cantidad de bloques en la blockchain.
   - chain: Forward Linked List de objetos clase Block
   Métodos:
   - get_latest_block(): Obtien el último bloques en la blockchain
   - add_block(): Agrega un bloque a la blockchain.
   - is_chain_valid(): Valida si la blockchain es válida (si los código hash previos de los bloques son correctos).
   - read_and_load_csv(): Lee un archivo .csv, genera transacciones, crea bloques y los agrega a la blockchain.
   - display(): Pinta la información de la blockchain
   - display_block(): Pinta la información de un bloque en específico.
2. Block
   Atributos:
   - index: Número de bloque en la blockchain
   - nonce: Número tal que hace que el código hash del bloque empiece en "000"
   - hash_code: Cadena de ecriptación generado por el algortimo sha-256
   - prev_hash_code: Puntero al código hash del bloque anterior
   - transactions: Conjunto de transacciones del bloque
   Métodos:
   - generate_hash_code(): Genera el código hash del bloque de acuerdo a la información de todos los atributos.
   - is_valid(): Verifica que el código hash del bloque sea el correcto al generarlo nuevamente.
   - proof_of_work(): Verifica que el código hash del bloque empiece en "000"
   - short_display(): Pinta la información del bloque
3. Transaction
   Atributos: (Propios de Bitcoin)
   - date: Tiempo de apertura
   - open: Precio del Bitcoin en el tiempo de apertura
   - high: Precio mayor en el periodo
   - lowest: Precio menor en el periodo
   - close: Precio de cierre
   - volume: Número de unidades intercambiadas en el mercado en el periodo
   - close_time: Tiempo de cierre
   - quote_asset_volume: Volumen de la moneda de cambio de Bitcoin (Como el dólar)
   - number_of_trades: Número de operaciones o intercambios
   - taker_buy_base_asset_volume: Monto recibido
   - taker_buy_quote_asset_volume: Monto recibido en la moneda de cambio
   Métodos:
   - get_data_as_string(): Obtene la data de todos los atributos de la transaccion en string.
   - display(): Pinta la información de la transaccion

Consideraciones:

- La blockchain está compuesta de un conjunto de bloques
- Cada bloque está compuesta por un conjunto de transacciones
- Cada transaccion tiene sus propios atributos dependiendo del contexto
- El primer bloque de la blockchain (el génesis) tiene asignado un código hash previo de "0"
- La información ingresada al bloque es manual o a través de un archivo .csv
- La capacidad de transacciones que puede ser almacenada en un bloque es de 100
- El algortimo de ecriptación es el sha-256, pero es implementado en archivos .cpp de acuerdo a un código en un blog de internet, ya que no nos fue bien al intentar instalar OpenSSL en Windows/MacOS.


Las estructuras de datos implementadas por nuestra cuenta fueron las siguientes:

1. Forward Linked List: Para almacenar los bloques dentro de la blockchain
2. Vector: Para almacenar las transacciones de un bloque

   ## Estrategia para asegurar integridad de su contenido

   La clase bloque tiene un método ```is_valid() ``` que compara si el código hash que tiene actualmente el bloque sería el mismo si se vuelve a generar el código hash (el cual toma en cuenta la información de todos los atributos de la clase bloque).

   ## Proof of Work

   El proof of work que utilizamos es que si alguien intenta agregar un bloque, primero debe encontrar un número (nonce) tal que haga que los primeros tres caracteres del código hash del bloque sean 0. Si se llega a encontrar ese número, recién se puede agregar el bloque a la cadena.

   ## Dataset utilizado

   El [dataset](https://www.kaggle.com/datasets/skanderhaddad/bitcoin-data?resource=download&select=data_description.txt) utilizado como ejemplo fue uno de data histórica de Bitcoin desde el 2017 hasta el 2021.

# Listas actividades para la parte 1

[Actividades](https://github.com/users/Neo-Zapata/projects/4/views/1)

# Referencias Bibliográficas

1. https://www.pwc.com/us/en/industries/financial-services/fintech/bitcoin-blockchain-cryptocurrency.html
2. https://andersbrownworth.com/blockchain/
3. https://bitcoin.org/bitcoin.pdf
4. https://www.euromoney.com/learning/blockchain-explained/how-transactions-get-into-the-blockchain
