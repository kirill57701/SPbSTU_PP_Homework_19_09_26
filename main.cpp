#include <iostream>
#include <random>
#include <vector>
#include <cstring>
#include <pthread.h>

struct SimConf_t
{
  double r;
  size_t ipt;
};

struct ThrSt_t
{
  const SimConf_t* conf;
  size_t seed;
  size_t s_h;
};

bool isInside(const double x, const double y, const double r)
{
  return (x * x + y * y <= r * r);
}

size_t calc(const double r, const size_t tests, const size_t seed)
{
  std::default_random_engine eng(seed);
  std::uniform_real_distribution< double > d(-r, r);
  size_t c = 0;
  for (size_t i = 0; i < tests; ++i) {
    c += isInside(d(eng), d(eng), r);
  }
  return c;
}

void* worker(void* arg)
{
  auto* st = static_cast< ThrSt_t* >(arg);
  st->s_h = calc(st->conf->r, st->conf->ipt, st->seed);
  return nullptr;
}

double area(const double r, const size_t threads, const size_t tests)
{
  const SimConf_t conf{r, tests / threads};
  std::vector< pthread_t > th(threads);
  std::vector< ThrSt_t > st(threads);
  std::random_device rd;

  const size_t seedStep = 1000;
  for (size_t i = 0; i < threads; ++i) {
    st[i] = {&conf, rd() + i * seedStep, 0};
    const int err = pthread_create(&th[i], nullptr, worker, &st[i]);
    if (err != 0) {
      std::cerr << "create err: " << std::strerror(err) << "\n";
    }
  }

  size_t tot = 0;
  for (size_t i = 0; i < threads; ++i) {
    const int err = pthread_join(th[i], nullptr);
    if (err != 0) {
      std::cerr << "join err: " << std::strerror(err) << "\n";
    }
    tot += st[i].s_h;
  }

  const size_t rem = tests % threads;
  if (rem > 0) {
    tot += calc(r, rem, rd());
  }

  const double boundingAreaMultiplier = 4.0;
  return (static_cast< double >(tot) / tests) * (boundingAreaMultiplier * r * r);
}

int main()
{
  const double r = 5.0;
  const size_t threads = 4;
  const size_t tests = 10000000;

  const double res = area(r, threads, tests);
  std::cout << "area: " << res << "\n";

  return 0;
}
