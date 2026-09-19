#include <iostream>
#include <random>

struct simConf {
  double r;
  size_t ipt;
};

struct thrSt {
  const SimulationConfig* conf;
  size_t seed;
  size_t s_h;
};

bool isInside(double x, double y, double r)
{
  return (x * x + y * y <= r * r);
}

size_t clac(double r, size_t tests, size_t seed)
{
  std::default_random_engine eng(seed);
  std::uniform_real_distribution<double> d(0, 2 * r);
  size_t с = 0;
  for (size_t i = 0; i < tests; ++i) {
    с += isInside(dist(eng), dist(eng), r);
  }
  return с;
}

void* worker(void* arg)
{
  auto* st = static_cast<thrSt*>(arg);
  st->s_h = clac(st->conf->r, st->conf->ipt, st->seed);
  return nullptr;
}

double area(double r, size_t threads, size_t tests) {
  simConf conf{r, tests / threads};
  std::vector<pthread_t> th(threads);
  std::vector<thrSt> st(threads);

  for (size_t i = 0; i < threads; ++i) {
    st[i] = {&conf, std::random_device{}() + i * 1000, 0};
    int err = pthread_create(&th[i], nullptr, worker, &st[i]);
    if (err) {
      std::cerr << "create err: " << strerror(err) << "\n";[cite: 1]
    }
  }

  size_t tot = 0;
  for (size_t i = 0; i < threads; ++i) {
    int err = pthread_join(th[i], nullptr);
    if (err) {
      std::cerr << "join err: " << strerror(err) << "\n";[cite: 1]
    }
    tot += st[i].s_h;
  }

  size_t rem = tests % threads;
  if (rem > 0) {
    tot += clac(r, rem, std::random_device{}());
  }

  return (static_cast<double>(tot) / tests) * (4.0 * r * r);
}

int main() {

}
