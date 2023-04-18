#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
getname(char *path)
{
	char *p;
	for (p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++;
	return p;
}

void
find(char *path, char *name)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	if (st.type == T_DIR) {
		if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
			printf("find: path too long\n");
			close(fd);
			return;
		}
		strcpy(buf, path);
		p = buf + strlen(buf);			
		*(p++) = '/';
		
		while (read(fd, &de, sizeof(de)) == sizeof(de)) {
			if (de.inum == 0)
				continue;

			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if (stat(buf, &st) < 0) {
				printf("find: cannot stat %s\n", buf);
				continue;
			}
	
			char *fname = getname(buf);
			switch (st.type) {
			case T_FILE:
				if (strcmp(fname, name) == 0) {
					printf("%s\n", buf);
				}
				break;
			case T_DIR:
				if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) {
					break;
				}
				find(buf, name);
			}
		}
	}
	close(fd);
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(2, "Bad! find <dir> <name>\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
