# Sleeping Barber Problem
Implementação da resolução do problema "Barbeiro Dorminhoco" (sleeping barber), problema de comunicação entre threads (IPC). Desenvolvido na disciplina de Sistemas Operacionais. 

## Apresentação do Problema
De maneira simplificada, o Problema da Barbearia consiste nos seguintes pontos:

- Se não houver clientes, o barbeiro irá dormir na sua cadeira;
- Quando um cliente chega, ele deve acordar o barbeiro (caso ele já não esteja atendendo);
- Se o barbeiro já estiver ocupado, o cliente pode:
- Esperar ele estar livre na sala de espera (se houver espaço);
- Ir embora (se não hoverem cadeiras livres).

## Problemáticas:
A principal complicação do problema são os riscos de corrida:

- Precisamos coordernar todos os clientes para não ter riscos de corrida
- Ações podem levar um determinado tempo e, se não estiverem sincronizadas, podem resultar em incoerência
- Dois clientes podem chegar à loja ao mesmo tempo, tendo apenas um lugar vazio
  
## Resolução do Problema
A resolução do problema baseia-se nos seguintes princípios:

Quando um cliente chega à barbearia, ele bloqueia o mutex e verifica o valor da variável clientes:
- Se for menor que n, ele incrementa 1
- Se for igual à n, ele sai da loja
Cliente envia sinal de chegada e espera o barbeiro.

Quando o cliente envia o sinal, o barbeiro acorda, atualiza o número de clientes e corta o cabelo; quando acaba, ele sinaliza que terminou o corte e espera o sinal de confirmação do cliente. Se outro cliente chegar enquanto o barbeiro está ocupado, na próxima iteração o barbeiro passa o semáforo do cliente sem dormir.

Quando o cliente recebe o sinal do barbeiro, ele espera o corte e sinal de término do barbeiro, depois manda seu sinal de término pro barbeiro (saiu do banco).
