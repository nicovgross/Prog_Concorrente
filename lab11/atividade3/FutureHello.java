/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;

//classe runnable
class MyCallable implements Callable<Long> {
  //construtor
  MyCallable() {
  }
 
  //método para execução
  public Long call() throws Exception {
    long s = 0;
    for (long i=1; i<=100; i++) {
      s++;
    }
    return s;
  }
}

// Tarefa que verifica se um número é primo
class Primo implements Callable<Integer> {
  private final int numero;

  Primo(int numero) {
      this.numero = numero;
  }

  // Retorna 1 se for primo, senão 0
  @Override
  public Integer call() {
    if (numero <= 1) return 0;
    if (numero == 2) return 1;
    if (numero % 2 == 0) return 0;
    for (int i = 3; i <= Math.sqrt(numero); i += 2) {
        if (numero % i == 0) return 0;
    }
    return 1;
  }
}

public class FutureHello {
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    if (args.length < 1) {
      System.out.println("Uso: java FuturePrimos <N>");
      System.exit(1);
    }

    int N = Integer.parseInt(args[0]);  // Limite superior do intervalo

    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    List<Future<Integer>> resultados = new ArrayList<>();

    // Submete uma tarefa para cada número de 1 até N
    for (int i = 1; i <= N; i++) {
      Callable<Integer> tarefa = new Primo(i);
      Future<Integer> futuro = executor.submit(tarefa);
      resultados.add(futuro);
    }

    int totalPrimos = 0;
    // Recupera os resultados
    for (Future<Integer> f : resultados) {
      try {
        totalPrimos += f.get();  // 1 se for primo, 0 se não for
      } catch (InterruptedException | ExecutionException e) {
        e.printStackTrace();
      }
    }

    System.out.println("Total de primos entre 1 e " + N + ": " + totalPrimos);
    executor.shutdown();
  }
}
