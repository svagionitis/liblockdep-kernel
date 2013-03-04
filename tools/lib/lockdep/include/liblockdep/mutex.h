#ifndef _LIBLOCKDEP_MUTEX_H
#define _LIBLOCKDEP_MUTEX_H

#ifdef __USE_LIBLOCKDEP

#include <pthread.h>
#include "common.h"

struct liblockdep_pthread_mutex {
	pthread_mutex_t mutex;
	struct lockdep_map dep_map;
};

typedef struct liblockdep_pthread_mutex liblockdep_pthread_mutex_t;

#define LIBLOCKDEP_PTHREAD_MUTEX_INITIALIZER(mtx)			\
		(const struct liblockdep_pthread_mutex) {		\
	.mutex = PTHREAD_MUTEX_INITIALIZER,				\
	.dep_map = STATIC_LOCKDEP_MAP_INIT(#mtx, &((&(mtx))->dep_map)),	\
}

static inline int __mutex_init(liblockdep_pthread_mutex_t *lock,
				const char *name,
				struct lock_class_key *key,
				const pthread_mutexattr_t *__mutexattr)
{
	lockdep_init_map(&lock->dep_map, name, key, 0);
	return pthread_mutex_init(&lock->mutex, __mutexattr);
}

#define liblockdep_pthread_mutex_init(mutex, mutexattr)		\
({								\
	static struct lock_class_key __key;			\
								\
	__mutex_init((mutex), #mutex, &__key, (mutexattr));	\
})

static inline int liblockdep_pthread_mutex_lock(liblockdep_pthread_mutex_t *lock)
{
	void *ip = _THIS_IP_;
	lock_acquire(&lock->dep_map, 0, 0, 0, 2, NULL, (unsigned long)ip);
	return pthread_mutex_lock(&lock->mutex);
}

static inline int liblockdep_pthread_mutex_unlock(liblockdep_pthread_mutex_t *lock)
{
	void *ip = _THIS_IP_;
	lock_release(&lock->dep_map, 0, (unsigned long)ip);
	return pthread_mutex_unlock(&lock->mutex);
}

static inline int liblockdep_pthread_mutex_trylock(liblockdep_pthread_mutex_t *lock)
{
	void *ip = _THIS_IP_;
	lock_acquire(&lock->dep_map, 0, 1, 0, 2, NULL, (unsigned long)ip);
	return pthread_mutex_trylock(&lock->mutex) == 0 ? 1 : 0;
}

static inline int liblockdep_pthread_mutex_destroy(liblockdep_pthread_mutex_t *lock)
{
	return pthread_mutex_destroy(&lock->mutex);
}

#define pthread_mutex_t         liblockdep_pthread_mutex_t
#define pthread_mutex_init      liblockdep_pthread_mutex_init
#define pthread_mutex_lock      liblockdep_pthread_mutex_lock
#define pthread_mutex_unlock    liblockdep_pthread_mutex_unlock
#define pthread_mutex_trylock   liblockdep_pthread_mutex_trylock
#define pthread_mutex_destroy   liblockdep_pthread_mutex_destroy

#endif

#endif
