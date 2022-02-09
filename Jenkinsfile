pipeline{
  agent{    
    docker{
      image "10.60.1.94:5000/gcc-analyse"
      args "-v /var/lib/jenkins/workspace/gcc-analyse:/home/gcc-analyse"
    }
  }
  stages{
    stage("Build"){
      steps{
        sh "make run"
        sh "gcov src/vf_lib/os/os_cirbuf.c"
      }
    }
  }
}
